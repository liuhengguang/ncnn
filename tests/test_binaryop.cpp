// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "testutil.h"

#include "layer/binaryop.h"

#define OP_TYPE_MAX 9

static int get_op_type()
{
    static int op_type = 0;
    if (op_type == OP_TYPE_MAX)
        op_type = 0;
    return op_type++;
}

static int test_binaryop(const ncnn::Mat& _a, const ncnn::Mat& _b)
{
    ncnn::Mat a = _a;
    ncnn::Mat b = _b;
    int op_type = get_op_type();
    if (op_type == 6)
    {
        // value must be positive for pow
        Randomize(a, 0.001f, 2.f);
        Randomize(b, 0.001f, 2.f);
    }

    ncnn::ParamDict pd;
    pd.set(0, op_type);
    pd.set(1, 0);// with_scalar
    pd.set(2, 0.f);// b

    std::vector<ncnn::Mat> weights(0);

    ncnn::Option opt;
    opt.num_threads = 1;
    opt.use_vulkan_compute = true;
    opt.use_int8_inference = false;

    std::vector<ncnn::Mat> ab(2);
    ab[0] = a;
    ab[1] = b;

    int ret = test_layer<ncnn::BinaryOp>("BinaryOp", pd, weights, opt, ab);
    if (ret != 0)
    {
        fprintf(stderr, "test_binaryop failed a.dims=%d a=(%d %d %d) b.dims=%d b=(%d %d %d) op_type=%d\n", a.dims, a.w, a.h, a.c, b.dims, b.w, b.h, b.c, op_type);
    }

    return ret;
}

static int test_binaryop(const ncnn::Mat& _a, float b)
{
    ncnn::Mat a = _a;
    int op_type = get_op_type();
    if (op_type == 6)
    {
        // value must be positive for pow
        Randomize(a, 0.001f, 2.f);
        b = RandomFloat(0.001f, 2.f);
    }

    ncnn::ParamDict pd;
    pd.set(0, op_type);
    pd.set(1, 1);// with_scalar
    pd.set(2, b);// b

    std::vector<ncnn::Mat> weights(0);

    ncnn::Option opt;
    opt.num_threads = 1;
    opt.use_vulkan_compute = true;
    opt.use_int8_inference = false;
    opt.use_fp16_packed = false;
    opt.use_fp16_storage = false;
    opt.use_fp16_arithmetic = false;
    opt.use_int8_storage = false;
    opt.use_int8_arithmetic = false;

    int ret = test_layer<ncnn::BinaryOp>("BinaryOp", pd, weights, opt, a);
    if (ret != 0)
    {
        fprintf(stderr, "test_binaryop failed a.dims=%d a=(%d %d %d) b=%f op_type=%d\n", a.dims, a.w, a.h, a.c, b, op_type);
    }

    return ret;
}

// https://github.com/Tencent/ncnn/wiki/binaryop-broadcasting

static int test_binaryop_1()
{
    return 0
        || test_binaryop(RandomMat(1), 1.f)
        ;
}

static int test_binaryop_2()
{
    return 0
        || test_binaryop(RandomMat(1), RandomMat(1))
        || test_binaryop(RandomMat(1), RandomMat(4))
        || test_binaryop(RandomMat(1), RandomMat(8))
        ;
}

static int test_binaryop_3()
{
    return 0
        || test_binaryop(RandomMat(1), RandomMat(2, 3))
        || test_binaryop(RandomMat(1), RandomMat(2, 4))
        || test_binaryop(RandomMat(1), RandomMat(2, 8))
        ;
}

static int test_binaryop_4()
{
    return 0
        || test_binaryop(RandomMat(1), RandomMat(2, 3, 2))
        || test_binaryop(RandomMat(1), RandomMat(2, 3, 4))
        || test_binaryop(RandomMat(1), RandomMat(2, 3, 8))
        ;
}

static int test_binaryop_5()
{
    return 0
        || test_binaryop(RandomMat(2), 1.f)
        || test_binaryop(RandomMat(4), 1.f)
        || test_binaryop(RandomMat(8), 1.f)
        ;
}

static int test_binaryop_6()
{
    return 0
        || test_binaryop(RandomMat(2), RandomMat(1))
        || test_binaryop(RandomMat(4), RandomMat(1))
        || test_binaryop(RandomMat(8), RandomMat(1))
        ;
}

static int test_binaryop_7()
{
    return 0
        || test_binaryop(RandomMat(2), RandomMat(2))
        || test_binaryop(RandomMat(4), RandomMat(4))
        || test_binaryop(RandomMat(8), RandomMat(8))
        ;
}

static int test_binaryop_8()
{
    return 0
        || test_binaryop(RandomMat(3), RandomMat(2, 3))
        || test_binaryop(RandomMat(4), RandomMat(2, 4))
        || test_binaryop(RandomMat(8), RandomMat(2, 8))
        ;
}

static int test_binaryop_9()
{
    return 0
        || test_binaryop(RandomMat(2), RandomMat(2, 3, 2))
        || test_binaryop(RandomMat(4), RandomMat(2, 3, 4))
        || test_binaryop(RandomMat(8), RandomMat(2, 3, 8))
        ;
}

static int test_binaryop_10()
{
    return 0
        || test_binaryop(RandomMat(2, 3), 1.f)
        || test_binaryop(RandomMat(2, 4), 1.f)
        || test_binaryop(RandomMat(2, 8), 1.f)
        ;
}

static int test_binaryop_11()
{
    return 0
        || test_binaryop(RandomMat(2, 3), RandomMat(1))
        || test_binaryop(RandomMat(2, 4), RandomMat(1))
        || test_binaryop(RandomMat(2, 8), RandomMat(1))
        ;
}

static int test_binaryop_12()
{
    return 0
        || test_binaryop(RandomMat(2, 3), RandomMat(3))
        || test_binaryop(RandomMat(2, 4), RandomMat(4))
        || test_binaryop(RandomMat(2, 8), RandomMat(8))
        ;
}

static int test_binaryop_13()
{
    return 0
        || test_binaryop(RandomMat(2, 3), RandomMat(2, 3))
        || test_binaryop(RandomMat(2, 4), RandomMat(2, 4))
        || test_binaryop(RandomMat(2, 8), RandomMat(2, 8))
        ;
}

static int test_binaryop_14()
{
    return 0
        || test_binaryop(RandomMat(3, 2), RandomMat(2, 3, 2))
        || test_binaryop(RandomMat(3, 4), RandomMat(2, 3, 4))
        || test_binaryop(RandomMat(3, 8), RandomMat(2, 3, 8))
        ;
}

static int test_binaryop_15()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), 1.f)
        || test_binaryop(RandomMat(2, 3, 4), 1.f)
        || test_binaryop(RandomMat(2, 3, 8), 1.f)
        ;
}

static int test_binaryop_16()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), RandomMat(1))
        || test_binaryop(RandomMat(2, 3, 4), RandomMat(1))
        || test_binaryop(RandomMat(2, 3, 8), RandomMat(1))
        ;
}

static int test_binaryop_17()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), RandomMat(2))
        || test_binaryop(RandomMat(2, 3, 4), RandomMat(4))
        || test_binaryop(RandomMat(2, 3, 8), RandomMat(8))
        ;
}

static int test_binaryop_18()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), RandomMat(3, 2))
        || test_binaryop(RandomMat(2, 3, 4), RandomMat(3, 4))
        || test_binaryop(RandomMat(2, 3, 8), RandomMat(3, 8))
        ;
}

static int test_binaryop_19()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), RandomMat(2, 3, 2))
        || test_binaryop(RandomMat(2, 3, 4), RandomMat(2, 3, 4))
        || test_binaryop(RandomMat(2, 3, 8), RandomMat(2, 3, 8))
        ;
}

static int test_binaryop_s1()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), RandomMat(1, 1, 2))
        || test_binaryop(RandomMat(2, 3, 4), RandomMat(1, 1, 4))
        || test_binaryop(RandomMat(2, 3, 8), RandomMat(1, 1, 8))
        ;
}

static int test_binaryop_s2()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 2), RandomMat(2, 3, 1))
        || test_binaryop(RandomMat(2, 3, 4), RandomMat(2, 3, 1))
        || test_binaryop(RandomMat(2, 3, 8), RandomMat(2, 3, 1))
        ;
}

static int test_binaryop_s3()
{
    return 0
        || test_binaryop(RandomMat(1, 1, 2), RandomMat(2, 3, 2))
        || test_binaryop(RandomMat(1, 1, 4), RandomMat(2, 3, 4))
        || test_binaryop(RandomMat(1, 1, 8), RandomMat(2, 3, 8))
        ;
}

static int test_binaryop_s4()
{
    return 0
        || test_binaryop(RandomMat(2, 3, 1), RandomMat(2, 3, 2))
        || test_binaryop(RandomMat(2, 3, 1), RandomMat(2, 3, 4))
        || test_binaryop(RandomMat(2, 3, 1), RandomMat(2, 3, 8))
        ;
}

int main()
{
    SRAND(7767517);

    return 0
        || test_binaryop_1()
        || test_binaryop_2()
        || test_binaryop_3()
        || test_binaryop_4()
        || test_binaryop_5()
        || test_binaryop_6()
        || test_binaryop_7()
        || test_binaryop_8()
        || test_binaryop_9()
        || test_binaryop_10()
        || test_binaryop_11()
        || test_binaryop_12()
        || test_binaryop_13()
        || test_binaryop_14()
        || test_binaryop_15()
        || test_binaryop_16()
        || test_binaryop_17()
        || test_binaryop_18()
        || test_binaryop_19()
        || test_binaryop_s1()
        || test_binaryop_s2()
        || test_binaryop_s3()
        || test_binaryop_s4()
        ;
}
