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

bool Tag::match(Tag *tag) {
	if (tag == nullptr || tag_path == SNAME("")) {
		return false;
	}

	return tag->get_tag_path() == tag_path;
}

void Tag::set_tag_path(StringName path) {
	if (path == SNAME("")) {
    	// UtilityFunctions::print("No path!");
		return;
	}

	TagDatabase *database = TagDatabase::get_singleton();
	
    TypedArray<StringName> path_arr = TagHelpers::split_path(path);
    TagTreeItem *tag = database->get_tag(path_arr);
	
	if (tag == nullptr) {
    	// UtilityFunctions::print("No tag found with path: " + UtilityFunctions::str(path_arr));
		return;
	}
	
	// UtilityFunctions::print("Path set.");
	tag_path = path; 
}

void Tag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("match", "Other Tag"), &Tag::match);

	ClassDB::bind_method(D_METHOD("set_tag_path"), &Tag::set_tag_path);
	ClassDB::bind_method(D_METHOD("get_tag_path"), &Tag::get_tag_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "tag_path"), "set_tag_path", "get_tag_path");
}
