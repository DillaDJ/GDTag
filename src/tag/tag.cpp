#include "tag.h"
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

TagTreeItem *Tag::get_tag() {
	TagDatabase *database = TagDatabase::get_singleton();

    TypedArray<StringName> path = TypedArray<StringName>();
    PackedStringArray elements = linked_tag_path.split("/");

    for (size_t i = 0; i < elements.size(); i++)
    {
        path.append(elements[i]);
    }    

    TagTreeItem *tag = database->get_tag(path);
    return tag;
}

void Tag::set_tag(TagTreeItem *tag) {
	
}

void Tag::_bind_methods() {
}
