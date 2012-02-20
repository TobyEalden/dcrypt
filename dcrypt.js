(function() {
  var Cipher, Decipher, Encode, Hash, Hmac, KeyPair, Random, Rsa, Sign, Verify, X509, X509CRL, dcrypt, _bindings;

  _bindings = require('./compiled/' + process.platform + '/' + process.arch + '/dcrypt');

  Random = _bindings.Random;

  Hash = _bindings.Hash;

  Sign = _bindings.Sign;

  Verify = _bindings.Verify;

  KeyPair = _bindings.KeyPair;

  Encode = _bindings.Encode;

  Cipher = _bindings.Cipher;

  Decipher = _bindings.Decipher;

  Rsa = _bindings.Rsa;

  Hmac = _bindings.Hmac;

  X509 = _bindings.X509;

  X509CRL = _bindings.X509CRL;

  dcrypt = {};

  dcrypt.random = {};

  dcrypt.random.randomBytes = function(len) {
    var buff, rb;
    len = len || 16;
    buff = new Buffer(len);
    rb = new Random();
    rb.randomBytes(buff);
    return buff;
  };

  exports.random = dcrypt.random;

  dcrypt.hash = Hash;

  exports.hash = {};

  exports.hash.createHash = function(hash) {
    return new Hash(hash);
  };

  dcrypt.sign = Sign;

  exports.sign = {};

  exports.sign.createSign = function(algo) {
    return (new Sign).init(algo);
  };

  dcrypt.verify = Verify;

  exports.verify = {};

  exports.verify.createVerify = function(algo) {
    return (new Verify).init(algo);
  };

  dcrypt.keypair = KeyPair;

  exports.keypair = {};

  exports.keypair.newRSA = function(size, exp) {
    size = size || 1024;
    exp = exp || 65537;
    return (new KeyPair).newRSA(size, exp);
  };

  exports.keypair.newECDSA = function(curve) {
    curve = curve || "secp256k1";
    return (new KeyPair).newECDSA(curve);
  };

  exports.keypair.parseECDSA = function(filename, public) {
    return (new KeyPair).parseECDSA(filename, public);
  };

  exports.keypair.parseRSA = function(filename, public) {
    return (new KeyPair).parseRSA(filename, public);
  };

  dcrypt.encode = Encode;

  exports.encode = {};

  exports.encode.encodeBase58 = function(data) {
    return (new Encode).encodeBase58(data);
  };

  exports.encode.decodeBase58 = function(data) {
    return (new Encode).decodeBase58(data);
  };

  dcrypt.cipher = Cipher;

  exports.cipher = {};

  exports.cipher.createCipher = function(cipher, key) {
    return (new Cipher).init(cipher, key);
  };

  exports.cipher.createCipheriv = function(cipher, key, iv) {
    return (new Cipher).initiv(cipher, key, iv);
  };

  dcrypt.decipher = Decipher;

  exports.decipher = {};

  exports.decipher.createDecipher = function(cipher, key) {
    return (new Decipher).init(cipher, key);
  };

  exports.cipher.createDecipheriv = function(cipher, key, iv) {
    return (new Decipher).initiv(cipher, key, iv);
  };

  dcrypt.rsa = Rsa;

  exports.rsa = {};

  exports.rsa.encrypt = function(pem_pub, msg, padding, out_encoding) {
    out_encoding = out_encoding || 'hex';
    padding = padding || 'RSA_PKCS1_PADDING';
    return (new Rsa).encrypt(pem_pub, msg, padding, out_encoding);
  };

  exports.rsa.decrypt = function(pem_priv, enc_msg, padding, in_encoding) {
    var out_encoding;
    out_encoding = out_encoding || 'hex';
    padding = padding || 'RSA_PKCS1_PADDING';
    return (new Rsa).decrypt(pem_priv, enc_msg, padding, in_encoding);
  };

  dcrypt.hmac = Hmac;

  exports.hmac = {};

  exports.hmac.createHmac = function(hmac, key) {
    return (new Hmac).init(hmac, key);
  };

  dcrypt.x509 = X509;

  dcrypt.x509CRL = X509CRL;

  exports.x509 = {};

  exports.x509.parseCert = function(cert, format) {
    return (new X509).parseCert(cert, format);
  };

  exports.x509.parseCrl = function(crl, format) {
    return (new X509CRL).parseCrl(crl, format);
  };

  exports.x509.createCert = function(args) {
    return (new X509).createCert(args);
  };

}).call(this);
