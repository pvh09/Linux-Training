#!/bin/bash
calculate_parentheses() {
    local input="$1"
    local result
    result=$(echo "$input" | bc)
    echo "$result"
}

# Main calculation function
calculate() {
    local expression="$1"
    local result="$expression"

    local pattern="\(([^()]*)\)"

    # Find innermost parentheses and calculate their contents until no more parentheses are found
    while [[ $result =~ $pattern ]];do
        inner_expr="${BASH_REMATCH[1]}"
        inner_result=$(calculate_parentheses "$inner_expr")
        result=${result//"${BASH_REMATCH[0]}"/$inner_result}
    done

    # Use bc to evaluate the final expression
    final_result=$(echo "$result" | bc)

    echo "$final_result"
}

# Check if at least one argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <expression>"
    exit 1
fi

expression="$*"
# Replace 'x' with '*' for multiplication
expression="${expression//x/*}"
result=$(calculate "$expression")

echo "Result: $result"

