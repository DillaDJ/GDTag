# GDTag
A simple tagging system for Godot, modeled after Unreal Engine's gameplay tag system.
The purpose of the system is to provide a nice way to easily handle string constants.

## Usage
### The Tag Editor
Open the tag editor to start: Project>Tools>Open Tag Editor
![Path to the editor](https://github.com/DillaDJ/GDTag/blob/main/doc/Editor-1.png)
The tag editor should now be docked on the top-left.

The toolbar has three buttons, clicking the first button will add a tag. Left clicking tags will select them.
Adding a tag with another tag selected will parent it to the selected tag.

To delete a tag, click either the second or third toolbar button with a tag selected.
The second button will delete a tag and preserve its children, whereas the third will delete it and all its children.

### GDScript
To actually make use of our tags, we do a little bit of scripting

#### Tags
TODO: Use the Tag type a single tag to an object

#### TagContainers
TODO: A TagContainer is useful if you want multiple tags on an object.

TODO: You can query a TagContainer with multiple methods...
