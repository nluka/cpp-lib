#ifndef CPPLIB_SEQUENCEGEN_TESTS_HPP
#define CPPLIB_SEQUENCEGEN_TESTS_HPP

#include "config.hpp"

#if TEST_SEQUENCEGEN

#include "../../include/lengthof.hpp"
#include "../../include/sequence-gen.hpp"
#include "../../include/test.hpp"

void seqgen_tests() {
  SETUP_SUITE_USING(seqgen::populate);

  auto const testCase = [&s](
    char const *const pattern,
    std::initializer_list<int> expected
  ) {
    size_t const len = expected.size();
    auto const output = std::unique_ptr<int []>(new int[len]);
    populate<int>(output.get(), pattern);
    s.assert(pattern, std::memcmp(output.get(), expected.begin(), len) == 0);
  };

  // single num(s)
  testCase("1", { 1 });
  testCase("1,2,3", { 1,2,3 });
  testCase("-1,-2,-3", { -1,-2,-3 });
  testCase("-1,7,9", { -1,7,9 });
  testCase("2345", { 2345 });
  testCase("-2345", { -2345 });

  // repeated num(s)
  testCase("1{3}", { 1,1,1 });
  testCase("-1{3}", { -1,-1,-1 });
  testCase("1{3},2{4}", { 1,1,1, 2,2,2,2 });

  // range(s)
  testCase("1..3", { 1,2,3 });
  testCase("-1..3", { -1,0,1,2,3 });
  testCase("-1..-3", { -1,-2,-3 });
  testCase("1..-3", { 1,0,-1,-2,-3 });
  testCase("-111..-113", { -111,-112,-113 });

  // all together
  testCase("-1{2}, 0..3, 9, 4..6", { -1,-1, 0,1,2,3, 9, 4,5,6 });
}

#endif // TEST_SEQUENCEGEN

#endif // CPPLIB_SEQUENCEGEN_TESTS_HPP
