# GDTag
A simple tagging system for Godot, modeled after Unreal Engine's gameplay tag system.
The purpose of the system is to provide a nice way to easily handle string constants.

## Usage
### Tags
To use a tag, create an export variable with a type of Tag in gdscript:
```gdscript
@export var tag_a : Tag
```

You may now use the tag editor to select a tag for this property:
![Tag editor](https://github.com/DillaDJ/GDTag/blob/main/doc/Tag-1.png)

This embedded TagEditor has all the functionality of the docked editor.
See below for more info on the TagEditor

The '[b]Tag[/b]' type may only select one tag at a time.
Use it to compare against other [b]Tags[/b] or [b]TagContainers[/b].

#### bool match(OtherTag: Tag)
Returns true if the compared [b]Tag[/b] [i]OtherTag[/i] matches the current tag exactly.

#### bool match_inheritance(OtherTag: Tag)
Returns true if all the compared [b]Tag[/b] OtherTag's path elements are contained in the current tag.
For example: The tag '[i]Deep/Parent/Child[/i]' contains 'Deep' then 'Parent' so it will match with tag: '[i]Deep/Parent[/i]'.

##### Use Case
A use case for this could be in an RPG: We have an [b]EnemyInfo[/b] resource that has a [b]Tag[/b] for enemy type: '[i]Enemies/Undead/Skeleton[/i]', skeleton enemies take extra blunt damage in our game so we can match the skeleton type exactly.

However, all undead types take damage from healing effects instead of restoring HP... We can use [b]enemy_type.match_inheritance('[i]Enemies/Undead/[/i]')[/b] to check this! This will match for our skeletons and also a new ghost type that we want to add in the future: '[i]Enemies/Undead/Ghost/[/i]'

### TagContainers
TODO: A TagContainer is useful if you want multiple tags on a property.

## The Tag Editor
To open the standalone tag editor: Project>Tools>Open Tag Editor
![Path to the editor](https://github.com/DillaDJ/GDTag/blob/main/doc/Editor-1.png)

The tag editor should now be docked on the top-left, you may move it but re-opening it will return it to its original location.

You may also open it by editing either an exported [b]Tag[/b] or [b]TagContainer[/b] in gdscript.

![Tag editor](https://github.com/DillaDJ/GDTag/blob/main/doc/Editor-2.png)
The toolbar has two buttons, clicking the first button will add a tag and prompt you to name it. 
Tags with the same parent may not share names. That is, every tag's path should be unique.

Left clicking tags will select them. 
Adding a tag with a tag selected will parent it to the selected tag.

To delete a tag, click the second toolbar button with a tag selected. 
This will delete the tag and all its children.

To rename a tag, double click it.

## Known issues
### Dupliced objects with tags will reference the same tag.
Since [b]Tags[/b] are Resources, a workaround is removing the reference in the new object under the scene file after duplication otherwise editing it will result in unwanted changes on the original object.