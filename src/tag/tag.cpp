#include "tag.h"
#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/internal_tag.hpp"

InternalTag *Tag::get_tag() {
	TagDatabase *database = TagDatabase::get_singleton();
    TypedArray<StringName> path_arr = TagHelpers::split_path(tag_path);   
    InternalTag *tag = database->get_tag(path_arr);
    return tag;
}

bool Tag::match(Tag *tag) {
	if (tag == nullptr || tag_path == SNAME("")) {
		return false;
	}

	return tag->get_tag_path() == tag_path;
}

bool Tag::match_inheritance(Tag *tag) {
	if (tag == nullptr || tag_path == SNAME("")) {
		return false;
	}

	TypedArray<StringName> other_path_arr = TagHelpers::split_path(tag->get_tag_path());
	TypedArray<StringName> path_arr = TagHelpers::split_path(tag_path);

	if (path_arr.size() == 0 || other_path_arr.size() == 0 || path_arr.size() < other_path_arr.size()) {
		return false;
	}
	
	for (size_t i = 0; i < other_path_arr.size(); i++)
	{
		StringName a = (StringName) path_arr[i];
		StringName b = (StringName) other_path_arr[i];

		if (a != b) {
			return false;
		}
	}
	
	return true;
}

void Tag::set_tag_path(StringName path) {
	if (path == SNAME("")) {
    	// UtilityFunctions::print("No path!");
		return;
	}

	TagDatabase *database = TagDatabase::get_singleton();
	
    TypedArray<StringName> path_arr = TagHelpers::split_path(path);
    InternalTag *tag = database->get_tag(path_arr);
	
	if (tag == nullptr) {
    	UtilityFunctions::push_error("No tag found with path: " + UtilityFunctions::str(path_arr));
		return;
	}
	
	// UtilityFunctions::print("Path set.");
	tag_path = path; 
}

void Tag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("match", "Other Tag"), &Tag::match);
	ClassDB::bind_method(D_METHOD("match_inheritance", "Other Tag"), &Tag::match_inheritance);

	ClassDB::bind_method(D_METHOD("set_tag_path"), &Tag::set_tag_path);
	ClassDB::bind_method(D_METHOD("get_tag_path"), &Tag::get_tag_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "tag_path"), "set_tag_path", "get_tag_path");
}
