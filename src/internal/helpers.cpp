#include "helpers.hpp"

StringName TagHelpers::merge_path(TypedArray<StringName> path_arr) {
    StringName path = "";

    for (size_t i = 0; i < path_arr.size(); i++)
    {
        StringName path_element = (StringName) path_arr[i];
        path = path + path_element + SNAME("/");
    }

    return path;
}

TypedArray<StringName> TagHelpers::split_path(StringName path) {
	TypedArray<StringName> path_arr = TypedArray<StringName>();
    PackedStringArray elements = path.split("/", false);

    for (size_t i = 0; i < elements.size(); i++)
    {
        path_arr.append(elements[i]);
    } 

    return path_arr;
}
