// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#ifndef GHFTEST_H_
#define GHFTEST_H_

// Definition
class GhfTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(GhfTest);
  CPPUNIT_TEST(testGetSymbol);
  CPPUNIT_TEST(testPltHook);
  CPPUNIT_TEST(testDetourUndetour);
  CPPUNIT_TEST(testGetSection);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testGetSection();
  void testGetSymbol();
  void testPltHook();
  void testDetourUndetour();
};

#define GHF_TEST_MARKER 1337

// For pltHook test
void *my_malloc(size_t size);

// For detour/undetour test
int functionToDetour();
int my_functionToDetour();

int main(int argc, char *argv[]);

#endif /*GHFTEST_H_*/
