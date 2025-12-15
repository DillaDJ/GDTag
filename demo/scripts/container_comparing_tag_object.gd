@tool
extends Node

@export var container : TagContainer
@export var tag : Tag

@export_tool_button("Check Container for Tag") 
var check_button : Callable = check_for_tag

func check_for_tag() -> void:
	var matched := container.has(tag)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
		
	var has_text : String = ("has" if matched == true else "does not have")
	
	print_rich("[b]{0}[/color][/b]: The container {1} the tag [b]'{2}'[/b]!"
		.format([match_text, has_text, tag.tag_path]))
