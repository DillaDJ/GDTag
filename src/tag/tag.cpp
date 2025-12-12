#include "tag.h"
#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

TagTreeItem *Tag::get_tag() {
	TagDatabase *database = TagDatabase::get_singleton();
    TypedArray<StringName> path_arr = TagHelpers::split_path(tag_path);   
    TagTreeItem *tag = database->get_tag(path_arr);
    return tag;
}

void Tag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_tag_path"), &Tag::set_tag_path);
	ClassDB::bind_method(D_METHOD("get_tag_path"), &Tag::get_tag_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "tag_path"), "set_tag_path", "get_tag_path");
}
