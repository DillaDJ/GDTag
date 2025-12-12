#include "tag_database.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include "internal/helpers.hpp"
#include "tag_tree_item.hpp"

TagDatabase* TagDatabase::singleton = nullptr;

TagDatabase *TagDatabase::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(TagDatabase);
    	singleton->initialize();
	}

	return singleton;
}

void TagDatabase::initialize() {
	nodes = Dictionary();
	read_from_file();
}

TagTreeItem *TagDatabase::get_tag(TypedArray<StringName> path_arr) {
	// UtilityFunctions::print("Getting tag with path: " + UtilityFunctions::str(path));

	if (path_arr.size() == 0 || !nodes.has(path_arr[0])) {
		// UtilityFunctions::print("Did not find tag...");
		return nullptr;
	}
	
	Variant v = nodes[path_arr[0]];
	TagTreeItem *current_tag = Object::cast_to<TagTreeItem>(v);

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

void TagDatabase::add_tag(StringName name, TagTreeItem* parent) {
	// UtilityFunctions::print("Adding tag: " + name);

	if (parent == nullptr) {
		if (nodes.has(name)) {
			return;
		}

		TagTreeItem* tag_item = memnew(TagTreeItem);
		tag_item->set_parent(nullptr);
		tag_item->set_name(name);
		nodes[name] = tag_item;
		
		emit_signal("tag_added");
		// UtilityFunctions::print("Tag added");
		return;
	}

	parent->add_child(name);
	emit_signal("tag_added");
	// UtilityFunctions::print("Tag added");
}

void TagDatabase::remove_tag(TagTreeItem *tag) {
	remove_tag_recursive(tag);
	write_to_file();

	emit_signal("tag_removed");
}

void TagDatabase::rename_tag(TagTreeItem* tag, StringName new_name) {
	TagTreeItem *parent = tag->get_parent();
    StringName name = tag->get_name();
	// UtilityFunctions::print("Renaming tag: " + name);

	if (parent == nullptr) {
		nodes.erase(name);
		tag->set_name(new_name);
		nodes[new_name] = tag;
		write_to_file();

		emit_signal("tag_renamed");
		// UtilityFunctions::print("Tag renamed");
		return;
	}

	parent->remove_child(name);
	tag->set_name(new_name);
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

void TagDatabase::remove_tag_recursive(TagTreeItem *tag) {
	TagTreeItem *parent = tag->get_parent();
    StringName name = tag->get_name();
	// UtilityFunctions::print("Removing tag: " + name);

	Array children = tag->get_children();
	for (size_t i = 0; i < children.size(); i++)
	{
		TagTreeItem *child_tag = cast_to<TagTreeItem>(children[i]);
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
	Ref<FileAccess> file = FileAccess::open("res://bin/tag_database.json", FileAccess::READ);
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
	load_tags(loaded_tags);
}

void TagDatabase::load_tags(Array loaded_tags) {
	for (size_t i = 0; i < loaded_tags.size(); i++)
	{
		Variant tag = loaded_tags[i];
		
		if (tag.get_type() == Variant::Type::ARRAY) {
			Array child_tags = (Array) tag; 
			
			StringName name = (StringName) child_tags[0];
			add_tag(name);
			
			TagTreeItem *loaded_tag = cast_to<TagTreeItem>(nodes[name]);
			loaded_tag->populate_children(child_tags[1]);

			continue;
		}

		if (tag.get_type() == Variant::Type::STRING || tag.get_type() == Variant::Type::STRING_NAME) {
			StringName name = (StringName) tag;
			add_tag(name);
		}
	}
}

void TagDatabase::write_to_file() {
	Ref<FileAccess> file = FileAccess::open("res://bin/tag_database.json", FileAccess::WRITE);
	
	String json = "";

	Array tags = nodes.keys();

	for (size_t i = 0; i < tags.size(); i++)
	{
		TagTreeItem *tag = cast_to<TagTreeItem>(nodes[tags[i]]);
		Array children = tag->get_children_names_recursive();
		
		if (children.size() > 0) {
			Array inner = Array();
			inner.append(tags[i]);
			inner.append(children);
			tags[i] = inner;
		}
	}
	
	json = JSON::stringify(tags, "\t");
	file->store_string(json);
	file->close();
}
