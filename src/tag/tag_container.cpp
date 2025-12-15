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

void TagContainer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_tag_path", "Path"), &TagContainer::add_tag_path);
	ClassDB::bind_method(D_METHOD("remove_tag_path", "Path"), &TagContainer::remove_tag_path);

	ClassDB::bind_method(D_METHOD("has"), &TagContainer::has);
	ClassDB::bind_method(D_METHOD("size"), &TagContainer::size);

	ClassDB::bind_method(D_METHOD("set_tag_paths_map"), &TagContainer::set_tag_paths_map);
	ClassDB::bind_method(D_METHOD("get_tag_paths_map"), &TagContainer::get_tag_paths_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "tag_paths"), "set_tag_paths_map", "get_tag_paths_map");
}