#!/bin/bash

# ==============================================================================
# POLYSET TESTER SCRIPT
# ==============================================================================
# This script compares your polyset implementation (in rendu/polyset) against
# the reference solution (in .resources/rank05/level1/polyset).
#
# It will:
#   1. Compile the reference solution
#   2. Compile your solution (using main.cpp from reference)
#   3. Run both and compare outputs
#   4. Check for memory leaks with valgrind
#
# Usage: ./tester.sh
# ==============================================================================

# ------------------------------------------------------------------------------
# STEP 1: Define color codes for pretty output
# ------------------------------------------------------------------------------
RED='\033[0;31m'      # Red for errors
GREEN='\033[0;32m'    # Green for success
YELLOW='\033[1;33m'   # Yellow for warnings/info
BLUE='\033[0;34m'     # Blue for section headers
NC='\033[0m'          # No Color (reset)

# ------------------------------------------------------------------------------
# STEP 2: Determine script location and set paths
# ------------------------------------------------------------------------------
# Get the directory where this script is located (reference solution directory)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Define path to user's solution (relative to script location)
USER_DIR="../../../../rendu/polyset"

# ------------------------------------------------------------------------------
# STEP 3: Create temporary directory for compilation and testing
# ------------------------------------------------------------------------------
# mktemp -d creates a unique temporary directory
TMP_DIR=$(mktemp -d)

# trap ensures the temp directory is deleted when script exits (cleanup)
trap "rm -rf $TMP_DIR" EXIT

# ------------------------------------------------------------------------------
# STEP 4: Print header and check if user solution exists
# ------------------------------------------------------------------------------
echo -e "${BLUE}🔍 Running COMPREHENSIVE TESTING for polyset${NC}"
echo "=========================================="
echo ""

# Verify that the user's solution directory exists
if [ ! -d "$USER_DIR" ]; then
    echo -e "${RED}❌ User solution folder not found: $USER_DIR${NC}"
    echo -e "${YELLOW}Please create your solution in: rendu/polyset/${NC}"
    exit 1
fi

# ------------------------------------------------------------------------------
# STEP 5: Compile the REFERENCE solution
# ------------------------------------------------------------------------------
# This creates a "known good" executable to compare against
# We use the SUBJECT's original files (array_bag, tree_bag, bag, searchable_bag)
# combined with our reference implementations (searchable_array_bag, searchable_tree_bag, set)
echo -e "${BLUE}📦 Step 1: Compiling reference solution...${NC}"

# Copy the SUBJECT's original files (these are the provided files students start with)
cp subject/*.cpp subject/*.hpp "$TMP_DIR/" 2>/dev/null

# Copy our reference implementations for the parts students need to implement
cp searchable_array_bag.cpp searchable_array_bag.hpp "$TMP_DIR/" 2>/dev/null
cp searchable_tree_bag.cpp searchable_tree_bag.hpp "$TMP_DIR/" 2>/dev/null
cp set.cpp set.hpp "$TMP_DIR/" 2>/dev/null

cd "$TMP_DIR"

# Compile with strict C++98 standard and all warnings as errors
g++ -Wall -Wextra -Werror -std=c++98 -o ref_polyset \
    array_bag.cpp \
    main.cpp \
    searchable_array_bag.cpp \
    searchable_tree_bag.cpp \
    set.cpp \
    tree_bag.cpp

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Reference compilation failed!${NC}"
    echo -e "${YELLOW}This is a bug in the reference solution - please report it.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Reference compilation successful!${NC}"
echo ""

# ------------------------------------------------------------------------------
# STEP 6: Compile the USER solution
# ------------------------------------------------------------------------------
# Copy user's .cpp and .hpp files (except main.cpp) over the reference files
# This tests user's implementation with the reference main.cpp
echo -e "${BLUE}📦 Step 2: Compiling your solution...${NC}"

cd "$SCRIPT_DIR"

# Copy user's .cpp files (but NOT main.cpp - we use reference's main.cpp)
for f in $USER_DIR/*.cpp; do
    filename=$(basename "$f")
    if [ "$filename" != "main.cpp" ]; then
        echo "   Copying user file: $filename"
        cp "$f" "$TMP_DIR/" 2>/dev/null
    fi
done

# Copy all user's .hpp header files
for f in $USER_DIR/*.hpp; do
    if [ -f "$f" ]; then
        filename=$(basename "$f")
        echo "   Copying user header: $filename"
        cp "$f" "$TMP_DIR/" 2>/dev/null
    fi
done

cd "$TMP_DIR"

# Compile user's solution with same flags
echo ""
echo "   Compiling with: g++ -Wall -Wextra -Werror -std=c++98"
g++ -Wall -Wextra -Werror -std=c++98 -o user_polyset \
    array_bag.cpp \
    main.cpp \
    searchable_array_bag.cpp \
    searchable_tree_bag.cpp \
    set.cpp \
    tree_bag.cpp 2>&1

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Your solution failed to compile!${NC}"
    echo -e "${YELLOW}Fix compilation errors above and try again.${NC}"
    exit 1
fi

echo -e "${GREEN}✅ Your solution compiled successfully!${NC}"
echo ""

# ------------------------------------------------------------------------------
# STEP 7: Run both solutions and capture output
# ------------------------------------------------------------------------------
echo -e "${BLUE}🚀 Step 3: Running tests with arguments: 1 2 3 4 5${NC}"
echo ""

# Run reference solution and capture output (both stdout and stderr)
./ref_polyset 1 2 3 4 5 > ref_output.txt 2>&1

# Run user solution and capture output
./user_polyset 1 2 3 4 5 > user_output.txt 2>&1

# ------------------------------------------------------------------------------
# STEP 8: Compare outputs using diff
# ------------------------------------------------------------------------------
echo -e "${BLUE}📊 Step 4: Comparing outputs...${NC}"
echo ""

# Initialize tracking variable
output_match=true

# diff -q does a quick comparison (just reports if different)
if diff -q ref_output.txt user_output.txt > /dev/null; then
    echo -e "${GREEN}✅ Output matches reference solution!${NC}"
else
    echo -e "${RED}❌ Output does NOT match reference solution!${NC}"
    echo ""
    
    # Show detailed comparison
    echo -e "${YELLOW}--- Reference Output (expected) ---${NC}"
    cat ref_output.txt
    echo ""
    
    echo -e "${YELLOW}--- Your Output (actual) ---${NC}"
    cat user_output.txt
    echo ""
    
    echo -e "${YELLOW}--- Differences (diff) ---${NC}"
    # diff shows what's different between the two files
    diff ref_output.txt user_output.txt
    
    output_match=false
fi
echo ""

# ------------------------------------------------------------------------------
# STEP 9: Check for memory leaks using Valgrind
# ------------------------------------------------------------------------------
echo -e "${BLUE}🔍 Step 5: Checking for memory leaks with Valgrind...${NC}"
echo ""

# Initialize valgrind tracking variables
has_leaks=false
has_errors=false

# Check if valgrind is installed
if ! command -v valgrind &> /dev/null; then
    echo -e "${YELLOW}⚠️ Valgrind not installed - skipping memory leak check${NC}"
    echo -e "${YELLOW}   Install with: sudo apt-get install valgrind${NC}"
else
    # Run valgrind with full leak checking
    # --leak-check=full: detailed leak information
    # --show-leak-kinds=all: show all types of leaks
    # --track-origins=yes: show where uninitialized values came from
    # --error-exitcode=42: return exit code 42 if errors found
    valgrind_output=$(valgrind \
        --leak-check=full \
        --show-leak-kinds=all \
        --track-origins=yes \
        --error-exitcode=42 \
        ./user_polyset 1 2 3 4 5 2>&1)
    
    valgrind_exit=$?
    
    # Check for memory leaks in output
    if echo "$valgrind_output" | grep -q "definitely lost:" && \
       ! echo "$valgrind_output" | grep "definitely lost: 0 bytes" > /dev/null; then
        has_leaks=true
    fi
    if echo "$valgrind_output" | grep -q "indirectly lost:" && \
       ! echo "$valgrind_output" | grep "indirectly lost: 0 bytes" > /dev/null; then
        has_leaks=true
    fi
    
    # Check for errors
    if echo "$valgrind_output" | grep -q "ERROR SUMMARY" && \
       ! echo "$valgrind_output" | grep "ERROR SUMMARY: 0 errors" > /dev/null; then
        has_errors=true
    fi
    
    # Display results
    if [ "$has_leaks" = false ]; then
        echo -e "${GREEN}✅ No memory leaks detected!${NC}"
    else
        echo -e "${RED}❌ Memory leaks detected!${NC}"
        echo ""
        echo -e "${YELLOW}--- Valgrind Leak Summary ---${NC}"
        echo "$valgrind_output" | grep -A5 "LEAK SUMMARY"
    fi
    
    if [ "$has_errors" = true ]; then
        echo -e "${RED}❌ Valgrind errors detected!${NC}"
        echo ""
        echo -e "${YELLOW}--- Valgrind Error Summary ---${NC}"
        echo "$valgrind_output" | grep -A2 "ERROR SUMMARY"
    fi
fi
echo ""

# ------------------------------------------------------------------------------
# STEP 10: Additional test cases
# ------------------------------------------------------------------------------
echo -e "${BLUE}🚀 Step 6: Running additional test cases...${NC}"
echo ""

# Test case 2: Different numbers
echo "Test case: 10 20 30 40 50"
./ref_polyset 10 20 30 40 50 > ref_output2.txt 2>&1
./user_polyset 10 20 30 40 50 > user_output2.txt 2>&1

if diff -q ref_output2.txt user_output2.txt > /dev/null; then
    echo -e "${GREEN}   ✅ Passed${NC}"
else
    echo -e "${RED}   ❌ Failed${NC}"
    output_match=false
fi

# Test case 3: Duplicate values
echo "Test case: 5 5 5 3 3 1"
./ref_polyset 5 5 5 3 3 1 > ref_output3.txt 2>&1
./user_polyset 5 5 5 3 3 1 > user_output3.txt 2>&1

if diff -q ref_output3.txt user_output3.txt > /dev/null; then
    echo -e "${GREEN}   ✅ Passed${NC}"
else
    echo -e "${RED}   ❌ Failed${NC}"
    output_match=false
fi

# Test case 4: Negative numbers
echo "Test case: -5 -3 -1 0 1 3 5"
./ref_polyset -5 -3 -1 0 1 3 5 > ref_output4.txt 2>&1
./user_polyset -5 -3 -1 0 1 3 5 > user_output4.txt 2>&1

if diff -q ref_output4.txt user_output4.txt > /dev/null; then
    echo -e "${GREEN}   ✅ Passed${NC}"
else
    echo -e "${RED}   ❌ Failed${NC}"
    output_match=false
fi

# Test case 5: Single element
echo "Test case: 42"
./ref_polyset 42 > ref_output5.txt 2>&1
./user_polyset 42 > user_output5.txt 2>&1

if diff -q ref_output5.txt user_output5.txt > /dev/null; then
    echo -e "${GREEN}   ✅ Passed${NC}"
else
    echo -e "${RED}   ❌ Failed${NC}"
    output_match=false
fi

echo ""

# ------------------------------------------------------------------------------
# STEP 11: Print final summary
# ------------------------------------------------------------------------------
echo "=========================================="
echo -e "${YELLOW}📊 FINAL SUMMARY${NC}"
echo "=========================================="

echo -n "Output Match:     "
if [ "$output_match" = true ]; then
    echo -e "${GREEN}PASSED${NC}"
else
    echo -e "${RED}FAILED${NC}"
fi

echo -n "Memory Leaks:     "
if [ "$has_leaks" = false ]; then
    echo -e "${GREEN}NONE${NC}"
else
    echo -e "${RED}DETECTED${NC}"
fi

echo -n "Valgrind Errors:  "
if [ "$has_errors" = false ]; then
    echo -e "${GREEN}NONE${NC}"
else
    echo -e "${RED}DETECTED${NC}"
fi

echo ""
echo "=========================================="
if [ "$output_match" = true ] && [ "$has_leaks" = false ] && [ "$has_errors" = false ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED! Great job!${NC}"
    exit_code=0
else
    echo -e "${RED}❌ SOME TESTS FAILED - Review output above${NC}"
    exit_code=1
fi
echo "=========================================="

# Wait for user input before exiting
read -rp "Press Enter to continue..."

exit $exit_code
