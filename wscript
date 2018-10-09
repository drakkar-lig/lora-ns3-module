
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('lora', ['network','mobility', 'energy'])
    module.source = [
        'model/lora-channel.cc',
        'model/lora-phy-gen.cc',
        'model/lora-mac.cc',
        'model/lora-transducer.cc',
        'model/lora-transducer-hd.cc',
        'model/lora-address.cc',
        'model/lora-net-device.cc',
        'model/lora-tx-mode.cc',
        'model/lora-prop-model.cc',
        'model/lora-prop-model-ideal.cc',
        'model/mac-lora-gw.cc',
        'model/lora-phy-dual.cc',
        'model/lora-header-common.cc',
        'model/lora-noise-model-default.cc',
        'model/lora-prop-model-thorp.cc',
        'model/lora-phy.cc',
        'model/lora-noise-model.cc',
        ]

    module_test = bld.create_ns3_module_test_library('lora')
    module_test.source = [
        'test/lora-test.cc', 
        ]

    headers = bld(features='ns3header')
    headers.module = 'lora'
    headers.source = [
        'model/lora-channel.h',
        'model/lora-phy.h',
        'model/lora-mac.h',
        'model/lora-net-device.h',
        'model/lora-prop-model.h',
        'model/lora-tx-mode.h',
        'model/lora-transducer.h',
        'model/lora-phy-gen.h',
        'model/lora-transducer-hd.h',
        'model/lora-address.h',
        'model/lora-prop-model-ideal.h',
        'model/mac-lora-gw.h',
        'model/lora-phy-dual.h',
        'model/lora-header-common.h',
        'model/lora-noise-model.h',
        'model/lora-noise-model-default.h',
        'model/lora-prop-model-thorp.h',
        ]


    if (bld.env['ENABLE_EXAMPLES']):
      bld.recurse('examples')

#    bld.ns3_python_bindings()
