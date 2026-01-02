#include "tag.h"
#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/internal_tag.hpp"

void Tag::set_tag(Tag *tag) {
	link_internal_tag(tag->get_internal_tag());
}

InternalTag *Tag::get_internal_tag() {
	TagDatabase *database = TagDatabase::get_singleton();
	InternalTag *tag = database->get_tag(linked_id);
    return tag;
}

void Tag::link_internal_tag(InternalTag *internal_tag) {
	linked_id = internal_tag->get_id();
}

bool Tag::match(Tag *tag) {
	if (tag == nullptr || linked_id == -1) {
		return false;
	}

	return tag->linked_id == linked_id;
}

bool Tag::match_inheritance(Tag *tag) {
	if (tag == nullptr || linked_id == -1) {
		return false;
	}

	TypedArray<StringName> other_path_arr = TagHelpers::split_path(tag->get_tag_path());
	TypedArray<StringName> path_arr = TagHelpers::split_path(get_tag_path());

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

StringName Tag::get_tag_path() {
	TagDatabase *database = TagDatabase::get_singleton();
	InternalTag *tag = database->get_tag(linked_id);
	return tag->get_path();
}

int Tag::get_linked_id() {
	return linked_id;
}

void Tag::_bind_methods() {
	ClassDB::bind_method(D_METHOD("match", "Other Tag"), &Tag::match);
	ClassDB::bind_method(D_METHOD("match_inheritance", "Other Tag"), &Tag::match_inheritance);
	
	ClassDB::bind_method(D_METHOD("get_tag_path"), &Tag::get_tag_path);
	ClassDB::bind_method(D_METHOD("set_tag", "Other Tag"), &Tag::set_tag);

	ClassDB::bind_method(D_METHOD("_link_internal_tag", "Internal Tag"), &Tag::link_internal_tag);

	ClassDB::bind_method(D_METHOD("_get_linked_id"), &Tag::get_linked_id);
	ClassDB::bind_method(D_METHOD("_set_linked_id"), &Tag::set_linked_id);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "_linked_id"), "_set_linked_id", "_get_linked_id");
}

void Tag::set_linked_id(int value) {
	linked_id = value;
}
