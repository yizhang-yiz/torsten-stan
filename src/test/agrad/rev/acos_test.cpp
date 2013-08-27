#include <stan/agrad/rev/acos.hpp>
#include <test/agrad/util.hpp>
#include <gtest/gtest.h>

TEST(AgradRev,acos_var) {
  AVAR a = 0.68;
  AVAR f = acos(a);
  EXPECT_FLOAT_EQ(acos(0.68), f.val());

  AVEC x = createAVEC(a);
  VEC g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(-1.0/sqrt(1.0 - (0.68 * 0.68)), g[0]);
}

TEST(AgradRev,acos_1) {
  AVAR a = 1;
  AVAR f = acos(a);
  EXPECT_FLOAT_EQ((0.0),f.val());

  AVEC x = createAVEC(a);
  VEC g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(-1.0/sqrt(1.0 - (1 * 1)), g[0]); 
}

TEST(AgradRev,acos_neg_1) {
  AVAR a = -1;
  AVAR f = acos(a);
  EXPECT_FLOAT_EQ((3.14159265),f.val());

  AVEC x = createAVEC(a);
  VEC g;
  f.grad(x,g);
  EXPECT_FLOAT_EQ(-1.0/sqrt(1.0 - (1 * 1)), g[0]); 
}

TEST(AgradRev,acos_out_of_bounds) {
  double epsilon = 1e-10;
  AVAR a = 1 + epsilon;
  EXPECT_THROW(acos(a),std::domain_error)
    <<"acos(1 + epsilon) should throw error";

  a = -1 - epsilon;
  EXPECT_THROW(acos(a),std::domain_error)
    <<"acos(-1 - epsilon) should throw error";
}
