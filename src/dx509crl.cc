#include "dx509crl.h"
Persistent<FunctionTemplate> DX509CRL::constructor;

void DX509CRL::Initialize(Handle<Object> target) {
  HandleScope scope;

  constructor = Persistent<FunctionTemplate>::New(FunctionTemplate::New(DX509CRL::New));
  constructor->InstanceTemplate()->SetInternalFieldCount(1);
  constructor->SetClassName(String::NewSymbol("X509CRL"));

  NODE_SET_PROTOTYPE_METHOD(constructor, "parseCrl", parseCrl);
  Local<ObjectTemplate> proto = constructor->PrototypeTemplate();

  target->Set(String::NewSymbol("X509CRL"), constructor->GetFunction());
}

Handle<Value> DX509CRL::New(const Arguments &args) {
  HandleScope scope;
  DX509CRL *x = new DX509CRL();
  x->Wrap(args.This());
  return args.This();
}

Handle<Value> DX509CRL::parseCrl(const Arguments &args) {
  HandleScope scope;
  DX509CRL *dx509crl = ObjectWrap::Unwrap<DX509CRL>(args.This());
  
  ASSERT_IS_STRING_OR_BUFFER(args[0]);
  int format = 2; /* default to pem */
  char formatUtf[7];
  if(args.Length() > 1 && !args[1].IsEmpty()) {
    Local<String> formatString = args[1]->ToString();
    if(formatString->Utf8Length() > 6)
      return ThrowException(Exception::TypeError(String::New("Invalid format specification")));
    
    formatString->WriteUtf8(formatUtf, 6);
    if(!strncmp(formatUtf, "pem", 3))
      format = 2;
    else if(!strncmp(formatUtf, "base64", 6))
      format = 1;
    else if(!strncmp(formatUtf, "der", 3))
      format = 0;
    else
      return ThrowException(Exception::TypeError(String::New("Invalid format specification")));
  }
  
  ssize_t crl_len = DecodeBytes(args[0], BINARY);
  char* crl_buf = new char[crl_len];
  ssize_t written = DecodeWrite(crl_buf, crl_len, args[0], BINARY);
  assert(crl_len = written);
  
  int ok = dx509crl->load_crl(crl_buf, crl_len, format, &dx509crl->x509_crl_);
  if(!ok)
    return ThrowException(Exception::Error(String::New("Unable to read CRL")));
  
  X509_CRL* x = dx509crl->x509_crl_;

  //node symbols
  Persistent<String> version_symbol        = NODE_PSYMBOL("version");
  Persistent<String> signature_algo_symbol = NODE_PSYMBOL("signature_algorithm");
  Persistent<String> issuer_symbol         = NODE_PSYMBOL("issuer");
  Persistent<String> last_update_symbol    = NODE_PSYMBOL("last_update");
  Persistent<String> next_update_symbol    = NODE_PSYMBOL("next_update");
  Persistent<String> signature_symbol      = NODE_PSYMBOL("signature");

  Local<Object> info = args.This();

  //issuer name
  X509_NAME *issuer = X509_CRL_get_issuer(x);
  if(issuer) {
    char *details = X509_NAME_oneline(issuer, 0, 0);
    info->Set(issuer_symbol, String::New(details));
    OPENSSL_free(details);
  }
  
  //Version
  long l;
  l = X509_CRL_get_version(x)+1;
  info->Set(version_symbol, Integer::New(l));
  
  char buf [256];
  BIO* bio = BIO_new(BIO_s_mem());
  memset(buf, 0, sizeof(buf));
  
  //last update
  ASN1_TIME *lastUpdate = X509_CRL_get_lastUpdate(x);
  if(lastUpdate) {
    ASN1_TIME_print(bio, lastUpdate);
    BIO_read(bio, buf, sizeof(buf)-1);
    info->Set(last_update_symbol, String::New(buf));
  }
  
  //next update
  ASN1_TIME *nextUpdate = X509_CRL_get_nextUpdate(x);
  if(nextUpdate) {
    ASN1_TIME_print(bio, X509_CRL_get_nextUpdate(x));
    BIO_read(bio, buf, sizeof(buf)-1);
    info->Set(next_update_symbol, String::New(buf));
  }
  
  //Signature Algorithm
  int wrote = i2a_ASN1_OBJECT(bio, x->crl->sig_alg->algorithm);
  BIO_read(bio, buf, sizeof(buf)-1);
  buf[wrote] = '\0';
  info->Set(signature_algo_symbol, String::New(buf));
  
  //Signature
  BIO *sig_bio = BIO_new(BIO_s_mem());
  ASN1_STRING *sigh = x->signature; 
  unsigned char *s;
  unsigned int n1 = sigh->length;
  s = sigh->data;
  for (int i=0; i<n1; i++) {
    BIO_printf(sig_bio, "%02x%s", s[i], ((i+1) == n1) ? "":":");
  }
  size_t sig_buf_len = n1*3;
  char* sig_buf = new char[sig_buf_len];
  BIO_read(sig_bio, sig_buf, sig_buf_len-1);
  info->Set(signature_symbol, String::New(sig_buf));
  delete [] sig_buf;
  delete [] crl_buf;
  if (bio != NULL) BIO_free(bio);
  return scope.Close(info);
}

/* format values:
 * 0: DER-encoded x509v3
 * 1: base64 x509v3
 * 2: PEM
 */
int DX509CRL::load_crl(char *crl, int crl_len, int format, X509_CRL** x509crlp) {
  BIO *bp = BIO_new_mem_buf(crl, crl_len);
  X509_CRL* x;
  if (format > 1) {
    x = PEM_read_bio_X509_CRL(bp, NULL, NULL, NULL);
  } else {
    if (format == 1) {
      BIO *b64;
      ASN1_VALUE *val;
      if(!(b64 = BIO_new(BIO_f_base64()))) {
        ASN1err(ASN1_F_B64_READ_ASN1,ERR_R_MALLOC_FAILURE);
        return 0;
      }
      bp = BIO_push(b64, bp);
    }
    x = d2i_X509_CRL_bio(bp, NULL);
  }
  
  if (x == NULL) {
    // ERR_print_errors(stderr);
    return 0;
  }
  if (bp != NULL) {
    BIO_free(bp);
  }
  *x509crlp = x;
  return 1;
}

DX509CRL::DX509CRL() : ObjectWrap() {}

DX509CRL::~DX509CRL() {
  fprintf(stderr, "Destructor called\n");
  X509_CRL_free(x509_crl_);
}

int DX509CRL::update_buf_len(const BIGNUM *b, size_t *pbuflen) {
	size_t i;
	if (!b)
		return 0;
	if (*pbuflen < (i = (size_t)BN_num_bytes(b)))
			*pbuflen = i;
}
