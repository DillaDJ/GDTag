#include "tag_database.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include "internal/helpers.hpp"
#include "internal_tag.hpp"

TagDatabase* TagDatabase::singleton = nullptr;

TagDatabase *TagDatabase::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(TagDatabase);
	}

	return singleton;
}

TagDatabase::TagDatabase() {
    initialize();
}

TagDatabase::~TagDatabase() {
	Array items = nodes.values();

	for (size_t i = 0; i < items.size(); i++)
	{
		InternalTag *node = cast_to<InternalTag>(items[i]);
		remove_tag_recursive(node);
	}
	
}

void TagDatabase::initialize() {
	nodes = Dictionary();
	next_id = 0;
	read_from_file();
}

void TagDatabase::uninitialize() {
	memdelete(singleton);
	singleton = nullptr;
}

InternalTag *TagDatabase::get_tag(int id) {
	return id_map.has(id) ? cast_to<InternalTag>(id_map[id]) : nullptr;
}

InternalTag *TagDatabase::get_tag(TypedArray<StringName> path_arr) {
	// UtilityFunctions::print("Getting tag with path: " + UtilityFunctions::str(path));

	if (path_arr.size() == 0 || !nodes.has(path_arr[0])) {
		// UtilityFunctions::print("Did not find tag...");
		return nullptr;
	}
	
	InternalTag *current_tag = Object::cast_to<InternalTag>(nodes[path_arr[0]]);

	for (size_t i = 0; i < path_arr.size(); i++)
	{
		if (i == 0) {
			continue;
		}

		current_tag = current_tag->get_child(path_arr[i]);

		if (current_tag == nullptr) {
			// UtilityFunctions::print("Did not find tag...");
			return nullptr;
		}
	}

	// UtilityFunctions::print("Found tag: " + (current_tag->get_name()));
	return current_tag;
}

InternalTag *TagDatabase::add_tag(StringName name, InternalTag* parent, bool recalculate) {
	// UtilityFunctions::print("Adding tag: " + name);

	if (parent == nullptr) {
		if (nodes.has(name)) {
			return nullptr;
		}

		InternalTag* tag_item = memnew(InternalTag);
		tag_item->set_parent(nullptr);
		tag_item->set_tag_name(name);

		if (recalculate) {
			tag_item->set_id(get_next_id(tag_item));
		}

		nodes[name] = tag_item;
		
		emit_signal("tag_added");
		// UtilityFunctions::print("Tag added");
		return tag_item;
	}

	InternalTag* tag_item = parent->add_child(name);
	
	if (recalculate) {
		tag_item->set_id(get_next_id(tag_item));
	}

	emit_signal("tag_added");
	// UtilityFunctions::print("Tag added");
	return tag_item;
}

void TagDatabase::remove_tag(InternalTag *tag) {
	remove_tag_recursive(tag);
	write_to_file();

	emit_signal("tag_removed");
}

void TagDatabase::rename_tag(InternalTag* tag, StringName new_name) {
	InternalTag *parent = tag->get_parent();
    StringName name = tag->get_tag_name();
	// UtilityFunctions::print("Renaming tag: " + name);

	if (parent == nullptr) {
		nodes.erase(name);
		tag->set_tag_name(new_name);
		nodes[new_name] = tag;
		write_to_file();

		emit_signal("tag_renamed");
		// UtilityFunctions::print("Tag renamed");
		return;
	}

	parent->remove_child(name);
	tag->set_tag_name(new_name);
	parent->add_child(tag);
	write_to_file();

	emit_signal("tag_renamed");
	// UtilityFunctions::print("Tag renamed");
}

void TagDatabase::_bind_methods() {
    ADD_SIGNAL(MethodInfo("tag_added"));
    ADD_SIGNAL(MethodInfo("tag_renamed"));
    ADD_SIGNAL(MethodInfo("tag_removed"));
}

int TagDatabase::get_next_id(InternalTag *tag) {
	while (id_map.has(next_id)) {
		next_id++;
	}

	id_map[next_id] = tag;
	return next_id;
}

void TagDatabase::remove_tag_recursive(InternalTag *tag) {
	InternalTag *parent = tag->get_parent();
    StringName name = tag->get_tag_name();
	// UtilityFunctions::print("Removing tag: " + name);

	Array children = tag->get_children();
	for (size_t i = 0; i < children.size(); i++)
	{
		InternalTag *child_tag = cast_to<InternalTag>(children[i]);
		// UtilityFunctions::print("Found child tag: " + child_tag->get_name());
		remove_tag_recursive(child_tag);
	}	
	
	if (parent == nullptr) {
		nodes.erase(name);
		memdelete(tag);
		// UtilityFunctions::print("Tag deleted");
		return;
	}

	parent->remove_child(name);
	memdelete(tag);
	// UtilityFunctions::print("Inner Tag deleted");
}

void TagDatabase::read_from_file() {
	Ref<FileAccess> file = FileAccess::open(get_file_path(), FileAccess::READ);
	if (file == nullptr) {
		// UtilityFunctions::print("Could not open tag_database.json");
		return;
	}

	String content = file->get_as_text();
	file->close();

	Variant result = JSON::parse_string(content);
	if (!result) {
		UtilityFunctions::push_error("Could not parse tag_database.json");
		return;
	}

	Array loaded_tags = (Array) result;
	load_tags_recursive(loaded_tags);
}

void TagDatabase::load_tags_recursive(Array loaded_tags, InternalTag *parent) {
	for (size_t i = 0; i < loaded_tags.size(); i++)
	{
		Dictionary tag = (Dictionary) loaded_tags[i];
		StringName name = tag["tag_name"];
		int id = tag["id"];
		Array children = tag["children"];

		InternalTag *loaded_tag = add_tag(name, parent);
		
		next_id = UtilityFunctions::max(id, next_id);
		loaded_tag->set_id(id);
		id_map[id] = loaded_tag;
		
		if (children.size() == 0) {
			continue;
		}

		InternalTag *next_parent = nullptr;
		if (parent == nullptr) {
			next_parent = cast_to<InternalTag>(nodes[name]);
		}
		else {
			next_parent = parent->get_child(name);
		}

		load_tags_recursive(children, next_parent);
	}
}

void TagDatabase::write_to_file() {
	Ref<FileAccess> file = FileAccess::open(get_file_path(), FileAccess::WRITE);
	if (file == nullptr) {
		UtilityFunctions::push_error("Could not open tag_database.json for writing");
		return;
	}
	
	String json = "";

	Array tags = get_children_recursive();
	
	json = JSON::stringify(tags, "\t");
	file->store_string(json);
	file->close();
}

Array TagDatabase::get_children_recursive(InternalTag *tag) {
	Array tags = Array();
	Array top_tag_names;

	if (tag == nullptr) {
		top_tag_names = nodes.keys();
	}
	else {
		top_tag_names = tag->get_child_names();
	}

	for (size_t i = 0; i < top_tag_names.size(); i++)
	{
		StringName tag_name = top_tag_names[i];
		InternalTag *internal_tag = nullptr;

		if (tag == nullptr) {
			internal_tag = cast_to<InternalTag>(nodes[tag_name]);
		}
		else {
			internal_tag = tag->get_child(tag_name);
		}

		if (internal_tag == nullptr){
			continue;
		}
		
		Dictionary save = Dictionary();
		save["id"] = internal_tag->get_id();
		save["tag_name"] = internal_tag->get_tag_name();
		save["children"] = get_children_recursive(internal_tag);

		tags.append(save);
	}

	return tags;
}

StringName TagDatabase::get_file_path() {
	ProjectSettings *settings = ProjectSettings::get_singleton();
	Variant path = settings->get_setting("GD_tag/tag_database_location");
	// UtilityFunctions::print(path);
	return (StringName) path + SNAME("/tag_database.json");
}
