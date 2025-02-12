project(haunt_examples)

function(add_example name)
    add_executable("haunt_example_${name}" "${name}/main.c")
    target_link_libraries("haunt_example_${name}" PRIVATE haunt::core)
endfunction(add_example)

add_example(001_minimal)
