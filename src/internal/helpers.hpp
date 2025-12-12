#pragma once

#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#define SNAME(m_arg) ([]() -> const StringName & { static StringName sname = StringName(m_arg, true); return sname; })()

using namespace godot;

class TagHelpers {

public:
    static StringName merge_path(TypedArray<StringName> path_arr);

    static TypedArray<StringName> split_path(StringName path);
};