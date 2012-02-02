
{
  'variables': {
      'node_module_sources': [
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
      ],
      'node_root': '/Users/paddy/data/work/dev/nodejs-0.6/',
      'node_root_win': 'c:\\node',
      'deps_root_win': 'c:\\dev2'
  },
  'targets': [
    {
      'target_name': 'dcrypt',
      'product_name': 'dcrypt',
      'type': 'loadable_module',
      'product_prefix': '',
      'product_extension':'node',
      'sources': [
        '<@(node_module_sources)',
      ],
      'defines': [
        'OPENSSL_NO_DEPRECATED',
        'PLATFORM="<(OS)"',
        '_LARGEFILE_SOURCE',
        '_FILE_OFFSET_BITS=64',
      ],
      'conditions': [
        [ 'OS=="mac"', {
          'libraries': [
             '-lssl',
             '-lcrypto',
             '-undefined dynamic_lookup',
          ],
          'xcode_settings': {
            'OTHER_LDFLAGS': [
              '-undefined dynamic_lookup',
            ]
          },
          'include_dirs': [
             'include/',
             '<@(node_root)/node/src',
             '<@(node_root)/node/deps/v8/include',
             '<@(node_root)/node/deps/uv/include',
          ],
        }],
        [ 'OS=="win"', {
          'defines': [
            'PLATFORM="win32"',
            '_WINDOWS',
            '__WINDOWS__', # ltdl
            'BUILDING_NODE_EXTENSION'
          ],
          'libraries': [ 
              'node.lib',
          ],
          'include_dirs': [
             'include',
             '<@(node_root_win)\\deps\\v8\\include',
             '<@(node_root_win)\\src',
             '<@(node_root_win)\\deps\\uv\\include',
          ],
          'msvs_settings': {
            'VCLinkerTool': {
              'AdditionalOptions': [
                # https://github.com/mapnik/node-mapnik/issues/74
                '/FORCE:MULTIPLE'
              ],
              'AdditionalLibraryDirectories': [
                '<@(node_root_win)\\Release\\lib',
                '<@(node_root_win)\\Release',
              ],
            },
          },
        },
      ], # windows
      ] # condition
    }, # targets
  ],
}
