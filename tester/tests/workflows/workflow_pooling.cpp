/*
Copyright (c) 2015, Intel Corporation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
* Neither the name of Intel Corporation nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "tester/common/workflows_for_tests.h"
#include "tester/tests/workflows/workflow_3layer_generator.h"

template < uint32_t x, uint32_t y, uint32_t z >
class workflow_pooling {
public:
    static nn_workflow_item_t* create_layer(nn_workflow_item_t* before, nn_device_interface_0_t *di) {
        nn_workflow_item_t *layer_to_be_done = nullptr;
        nn_workflow_use_descriptor_t inputs_descriptor = { before, 0 };
        di->workflow_item_create_function( &layer_to_be_done, 1, &inputs_descriptor, 1 );

        layer_to_be_done->name = "pooling";
        layer_to_be_done->type = NN_WORK_ITEM_TYPE_POOLING;
        layer_to_be_done->arguments.forward_pooling.mode = NN_POOLING_MODE_MAX;
        layer_to_be_done->arguments.forward_pooling.size[0] = 3;
        layer_to_be_done->arguments.forward_pooling.size[1] = 3;
        layer_to_be_done->arguments.forward_pooling.stride[0] = 2;
        layer_to_be_done->arguments.forward_pooling.stride[1] = 2;
        layer_to_be_done->output_format[0] = nn::output_format { x, y, z };

        return layer_to_be_done;
    }
};

// Code below creates 'attach_' object in anonymous namespace at global scope.
// This ensures, that object itself is not visible to other compilation units
// and it's constructor is ran befor main execution starts.
// The sole function of this construction is attaching this workflow builder to
// library of workflow builders (singleton command pattern).
namespace {
    const uint32_t x = 55, y = 55, z = 96;
    std::vector<uint32_t> in_size {x, y, z};
    std::vector<uint32_t> out_size {x/2, y/2, z};

    struct attach {
        workflow_3layer_generator< workflow_pooling< x/2, y/2, z > >* test_workflow;

        attach(std::vector<uint32_t> in_size, std::vector<uint32_t> out_size)
            : test_workflow(new workflow_3layer_generator< workflow_pooling< x/2, y/2, z > >( in_size, out_size ))
        {
            workflows_for_tests::instance().add( "workflow_pooling", test_workflow );
        }
        ~attach() { delete test_workflow; }
    };

    attach attach_(in_size, out_size);
}
