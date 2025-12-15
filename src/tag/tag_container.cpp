#include "tag_container.h"
#include "tag.h"

TagContainer::TagContainer() {
    tag_paths = Dictionary();
}

void TagContainer::add_tag_path(StringName path) {
	if (tag_paths.has(path)) {
        return;
    }

    tag_paths[path] = true;
    set_tag_paths_map(tag_paths);
}

void TagContainer::remove_tag_path(StringName path) {
    if (!tag_paths.has(path)) {
        return;
    }

    tag_paths.erase(path);
    set_tag_paths_map(tag_paths);
}

bool TagContainer::has(Tag *tag) {
	return tag_paths.has(tag->get_tag_path());
}

bool TagContainer::any(TagContainer *container) {
    TypedArray<StringName> paths = container->get_tag_paths();
    for (size_t i = 0; i < paths.size(); i++)
    {
        StringName path = (StringName) paths[i];
        if (tag_paths.has(path)) {
            return true;
        }
    }

	return false;
}

bool TagContainer::exact(TagContainer *container) {
    if (container->size() != size()) {
        return false;
    }

    TypedArray<StringName> paths = container->get_tag_paths();
    for (size_t i = 0; i < paths.size(); i++)
    {
        StringName path = (StringName) paths[i];
        if (!tag_paths.has(path)) {
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

    TypedArray<StringName> paths = container->get_tag_paths();
    for (size_t i = 0; i < paths.size(); i++)
    {
        StringName path = (StringName) paths[i];
        if (tag_paths.has(path)) {
            overlaps++;
        }
    }

	return overlaps;
}

void TagContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_tag_path", "Path"), &TagContainer::add_tag_path);
	ClassDB::bind_method(D_METHOD("remove_tag_path", "Path"), &TagContainer::remove_tag_path);

	ClassDB::bind_method(D_METHOD("has", "Other Tag"), &TagContainer::has);
	ClassDB::bind_method(D_METHOD("any", "Other Tag"), &TagContainer::any);
	ClassDB::bind_method(D_METHOD("exact", "Other Container"), &TagContainer::exact);
	ClassDB::bind_method(D_METHOD("all", "Other Container"), &TagContainer::all);
	ClassDB::bind_method(D_METHOD("none", "Other Container"), &TagContainer::none);
	ClassDB::bind_method(D_METHOD("overlap_count", "Other Container"), &TagContainer::overlap_count);
	ClassDB::bind_method(D_METHOD("size"), &TagContainer::size);

	ClassDB::bind_method(D_METHOD("set_tag_paths_map"), &TagContainer::set_tag_paths_map);
	ClassDB::bind_method(D_METHOD("get_tag_paths_map"), &TagContainer::get_tag_paths_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "tag_paths"), "set_tag_paths_map", "get_tag_paths_map");
}