#include "tag_container.h"
#include <godot_cpp/variant/dictionary.hpp>

#include "internal/internal_tag.hpp"
#include "tag.h"

TagContainer::TagContainer() {
    tag_ids = Dictionary();
}

void TagContainer::add_tag_internal(InternalTag *tag) {
    int id = tag->get_id();

	if (tag_ids.has(id)) {
        return;
    }

    tag_ids[id] = true;
}

void TagContainer::remove_tag_internal(InternalTag *tag) {
    int id = tag->get_id();

    if (!tag_ids.has(id)) {
        return;
    }

    tag_ids.erase(id);
}

bool TagContainer::has(Tag *tag) {
	return tag_ids.has(tag->get_linked_id());
}

bool TagContainer::any(TagContainer *container) {
    TypedArray<int> ids = container->get_tag_ids();

    for (size_t i = 0; i < ids.size(); i++)
    {
        int id = (int) ids[i];
        if (tag_ids.has(id)) {
            return true;
        }
    }

	return false;
}

bool TagContainer::exact(TagContainer *container) {
    if (container->size() != size()) {
        return false;
    }

    TypedArray<StringName> ids = container->get_tag_ids();
    for (size_t i = 0; i < ids.size(); i++)
    {
        int id = (int) ids[i];
        if (!ids.has(id)) {
            return false;
        }
    }

	return true;
}

bool TagContainer::all(TagContainer *container) {
	return overlap_count(container) == container->size();
}

bool TagContainer::none(TagContainer *container) {    
	return !any(container);
}

int TagContainer::overlap_count(TagContainer *container) {
    int overlaps = 0;

    TypedArray<StringName> ids = container->get_tag_ids();
    for (size_t i = 0; i < ids.size(); i++)
    {
        int id = (int) ids[i];
        if (ids.has(id)) {
            overlaps++;
        }
    }

	return overlaps;
}

void TagContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_add_tag_internal", "Internal Tag"), &TagContainer::add_tag_internal);
	ClassDB::bind_method(D_METHOD("_remove_tag_internal", "Internal Tag"), &TagContainer::remove_tag_internal);

	ClassDB::bind_method(D_METHOD("has", "Other Tag"), &TagContainer::has);
	ClassDB::bind_method(D_METHOD("any", "Other Tag"), &TagContainer::any);
	ClassDB::bind_method(D_METHOD("exact", "Other Container"), &TagContainer::exact);
	ClassDB::bind_method(D_METHOD("all", "Other Container"), &TagContainer::all);
	ClassDB::bind_method(D_METHOD("none", "Other Container"), &TagContainer::none);
	ClassDB::bind_method(D_METHOD("overlap_count", "Other Container"), &TagContainer::overlap_count);
	ClassDB::bind_method(D_METHOD("size"), &TagContainer::size);

	ClassDB::bind_method(D_METHOD("_set_tag_id_map"), &TagContainer::set_tag_id_map);
	ClassDB::bind_method(D_METHOD("_get_tag_id_map"), &TagContainer::get_tag_id_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "_tag_id_map"), "_set_tag_id_map", "_get_tag_id_map");
}