// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#include <dlfcn.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "ghf.h"
#include "ghf-test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(GhfTest);

void GhfTest::setUp() {}
void GhfTest::tearDown() {}

// Get ELF section test
void GhfTest::testGetSection() {
	// Init ELF ptr
	Elf *elf = initElf(NULL);
	CPPUNIT_ASSERT(elf != NULL);

	Elf_Scn *scn = NULL;

	scn = getSection(elf, "INEXISTANT_SECTION_NAME");
	CPPUNIT_ASSERT(scn == NULL);

	scn = getSection(elf, ".dynsym");
	CPPUNIT_ASSERT(scn != NULL);

	scn = getSection(elf, ".text");
	CPPUNIT_ASSERT(scn != NULL);
	
	// Get section headers
	GElf_Shdr *shdr = getSectionHeader(elf, ".text");
	CPPUNIT_ASSERT(shdr != NULL);

	CPPUNIT_ASSERT((unsigned int)main > shdr->sh_addr);
	CPPUNIT_ASSERT((unsigned int)main < shdr->sh_addr + shdr->sh_size);

	elf_end(elf);
}

// getSymbol test

char my_data1[512];
float my_data2[10];

void GhfTest::testGetSymbol() {
	GElf_Sym *sym;
	
	sym = getSymbol(NULL, "main");
	CPPUNIT_ASSERT(sym != NULL);
	CPPUNIT_ASSERT_EQUAL((void *)main, (void *)sym->st_value);

	sym = getSymbol(NULL, "NON_EXISTENT_SYMBOL");
	CPPUNIT_ASSERT(sym == NULL);

	sym = getSymbol(NULL, "my_data1");
	CPPUNIT_ASSERT(sym != NULL);
	CPPUNIT_ASSERT_EQUAL(sizeof(my_data1), (unsigned int)sym->st_size);

	sym = getSymbol(NULL, "my_data2");
	CPPUNIT_ASSERT(sym != NULL);
	CPPUNIT_ASSERT_EQUAL(sizeof(my_data2), (unsigned int)sym->st_size);

	sym = getSymbol(NULL, "strcmp");
	CPPUNIT_ASSERT(sym != NULL);
}

// pltHook test

void GhfTest::testPltHook() {
	int result = pltHook("malloc", (void *)my_malloc);
	CPPUNIT_ASSERT_EQUAL(1, result);
	CPPUNIT_ASSERT_EQUAL((int)malloc(GHF_TEST_MARKER), GHF_TEST_MARKER);

	// Test the original function	
	void *buf = malloc(256);
	CPPUNIT_ASSERT(buf != (void *)GHF_TEST_MARKER);
	CPPUNIT_ASSERT(buf != NULL);
}

void *my_malloc(size_t size) {
	// Get the original malloc
	static void *(*orig_malloc)(size_t size) = NULL;
	if (!orig_malloc) {
		orig_malloc = (void *(*)(size_t)) dlsym(RTLD_DEFAULT, "malloc");

		// Sanity check
		if (!orig_malloc)
			CPPUNIT_FAIL(dlerror());
	}

	if (size == GHF_TEST_MARKER)
		return (void *)GHF_TEST_MARKER;
	else
		return orig_malloc(size);
}

// detour/undetour test

void GhfTest::testDetourUndetour() {
	int (*orig_functionToDetour)();

	// Detour
	orig_functionToDetour = (int (*)()) detourFunction((void *)functionToDetour, (void *)my_functionToDetour);
	CPPUNIT_ASSERT_EQUAL(GHF_TEST_MARKER, functionToDetour());
	CPPUNIT_ASSERT_EQUAL(0, orig_functionToDetour());
	
	// Undetour
	undetourFunction((void *)functionToDetour, (void *)orig_functionToDetour);
	CPPUNIT_ASSERT_EQUAL(0, functionToDetour());
}

int functionToDetour() {
	return 0;
}

int my_functionToDetour() {
	return GHF_TEST_MARKER;
}

// main

int main(int argc, char *argv[]) {
	// Get the top level suite from the registry
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

	// Adds the test to the list of test to run
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);

	// Change the default outputter to a compiler error format outputter
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), std::cerr));

	// Run the tests
	bool wasSucessful = runner.run();

	// Return error code 1 if the one of test failed
	return !wasSucessful;
}
