#include "tag_editor_tree.hpp"

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/internal_tag.hpp"

TagEditorTree::TagEditorTree() {
    database = TagDatabase::get_singleton();
    mode = TagTreeMode::READ;
    
    set_hide_root(true);

    root_item = create_item();

    populate_tags_recursive(nullptr, nullptr);
}

void TagEditorTree::reset() {
	clear();

    root_item = create_item();
    populate_tags_recursive(nullptr, nullptr);
}

void TagEditorTree::set_mode(TagTreeMode p_mode) {
    set_columns(p_mode == TagTreeMode::READ ? 1 : 2);
    mode = p_mode; 
    
    if (p_mode == TagTreeMode::READ) { 
        return; 
    }

    set_column_expand(1, false);
}

TreeItem *TagEditorTree::create_tree_item(TreeItem *parent) {
// 	// UtilityFunctions::print("\nAdding new tag to " + (parent == root ? "root" : parent->get_text(0)) + "...");
    TreeItem *item = create_item(parent);
    
    if (mode != TagTreeMode::READ) {
        item->set_cell_mode(1, TreeItem::CELL_MODE_CHECK);
        item->set_editable(1, true);
    }

    return item;
}

void TagEditorTree::check_tag(TypedArray<StringName> path_arr) {
    if (mode == TagTreeMode::READ || path_arr.size() == 0) {
        return;
    }

    TreeItem *item = get_item_from_path_arr(path_arr);
    if (item == nullptr) {
        return;
    }

    item->set_checked(1, true);
}

void TagEditorTree::uncheck_tag(TypedArray<StringName> path_arr) {
    if (mode == TagTreeMode::READ || path_arr.size() == 0) {
        return;
    }

    TreeItem *item = get_item_from_path_arr(path_arr);
    if (item == nullptr) {
        return;
    }

    item->set_checked(1, false);
}

void TagEditorTree::uncheck_all_tags() {
    uncheck_recursive(root_item);
}

TypedArray<StringName> TagEditorTree::get_tag_path_arr(TreeItem *item) {
	TypedArray<StringName> path = TypedArray<StringName>();
    
    if (item == nullptr || item == root_item) {
        return path;
    }
    
    while (item != root_item) {
        path.append(item->get_text(0));
        item = item->get_parent();
    }

    path.reverse();
	return path;
}

TreeItem *TagEditorTree::get_item_from_path_arr(TypedArray<StringName> path_arr) {
    path_arr = path_arr.duplicate(); // Prevents passed in array from being modified

    // UtilityFunctions::print("Checking tag with path:");
    // UtilityFunctions::print(path_arr);

    if (root_item->get_child_count() == 0 || path_arr.size() == 0) {
        return nullptr;
    }

    StringName node = (StringName) path_arr.pop_front();
    TreeItem *child = root_item->get_first_child();

    while (child != nullptr)
    {
        // UtilityFunctions::print("\nNode: '" + node + "' Child: '" + child->get_text(0) + "'");

        if (child->get_text(0) == node) {
            if (path_arr.size() == 0) {
                child->set_checked(1, true);
                // UtilityFunctions::print("Found node to check!");
                return child;
            }

            // UtilityFunctions::print("Found node... getting first child.");
            child = child->get_first_child();
            node = (StringName) path_arr.pop_front();
        }
        else {
            child = child->get_next();
            // UtilityFunctions::print("Not this... getting next child");
        }
    }

    return nullptr;
}

Variant TagEditorTree::_get_drag_data(const Vector2 &p_at_position) {
    TreeItem *drag_item = get_item_at_position(p_at_position);
    if (drag_item == nullptr) {
        return nullptr;
    }
    
    drag_label = memnew(Label); // Seems to free itself, complains if you free it
    drag_label->set_text(drag_item->get_text(0));
    set_drag_preview(drag_label);

    InternalTag *tag = database->get_tag(get_tag_path_arr(drag_item));
	return tag;
}

bool TagEditorTree::_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) {
    set_drop_mode_flags(Tree::DROP_MODE_INBETWEEN | Tree::DROP_MODE_ON_ITEM);
	return true;
}

void TagEditorTree::_drop_data(const Vector2 &p_at_position, const Variant &p_data) {
    InternalTag *tag = cast_to<InternalTag>(p_data);
    if (tag == nullptr) {
        return;
    }

    TreeItem *drag_item = get_item_at_position(p_at_position);
    if (drag_item == nullptr) {
        // UtilityFunctions::print(tag->get_path() + SNAME(" to empty!"));
        database->move_tag(tag, nullptr, -100);
        return;
    }

    InternalTag *landing_tag = database->get_tag(get_tag_path_arr(drag_item));
    int drop_section = get_drop_section_at_position(p_at_position);
    database->move_tag(tag, landing_tag, drop_section);
}

void TagEditorTree::populate_tags_recursive(TreeItem *parent_item, InternalTag *parent_tag) {
    Array children = parent_tag == nullptr ? database->get_nodes() : parent_tag->get_children();

    for (size_t i = 0; i < children.size(); i++)
    {
        InternalTag *tag = cast_to<InternalTag>(children[i]);
        if (tag == nullptr) {
            return;
        }
        
        // UtilityFunctions::print(tag->get_tag_name());

        TreeItem *item = create_tree_item(parent_item == nullptr ? root_item : parent_item);
        item->set_text(0, tag->get_tag_name());
        
        populate_tags_recursive(item, tag);
    }
}

void TagEditorTree::uncheck_recursive(TreeItem *item) {
    if (item->get_child_count() == 0) {
        return;
    }

    TreeItem *child = item->get_first_child();

    while (child != nullptr) {
        child->set_checked(1, false);
        uncheck_recursive(child);
        child = child->get_next();
    }
}
