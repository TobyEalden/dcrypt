{
  'variables': {
    'openssl_Root': 'C:/OpenSSL-Win32',
  },
  'targets': [
    {
      'target_name': 'dcrypt',
      'sources': [
        'src/cipher.cc',
        'src/common.cc',
        'src/decipher.cc',
        'src/drsa.cc',
        'src/dx509.cc',
        'src/encode.cc',
        'src/hash.cc',
        'src/hmac.cc',
        'src/init.cc',
        'src/keypair.cc',
        'src/random.cc',
        'src/sign.cc',
		'src/verify.cc',
		'src/dx509crl.cc'
      ],
      'conditions': [
        [ 'OS=="win"', {
          #we need to link to the libeay32.lib
		  'defines': [ 'NOCRYPT' ],
          'libraries': ['-l<(openssl_Root)/lib/libeay32.lib', '-l<(openssl_Root)/lib/ssleay32.lib' ],
          'include_dirs': ['<(openssl_Root)/include'],
        }],
        [ 'OS=="freebsd" or OS=="openbsd" or OS=="solaris" or (OS=="linux")', {
          'libraries': ['-lssl', '-lcrypto'],
        }],	  
	  ],
      'dependencies': [        
      ]
    }
  ]
}
