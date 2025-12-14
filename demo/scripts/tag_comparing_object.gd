@tool
extends Node

@export var tag_a : Tag
@export var tag_b : Tag

@export_category("Actions")

@export_tool_button("Compare A and B") 
var compare_a_b_action : Callable = compare_a_b

@export_tool_button("Compare A's inheritance with B") 
var compare_a_b_inheret_action : Callable = compare_a_b_inheret

func compare_a_b() -> void:
	var matched := tag_a.match(tag_b)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
	
	print_rich("[b]{1}[/color][/b]: Between [b]'{0}'[/b] and [b]'{2}'[/b]!"
		.format([tag_a.tag_path, match_text, tag_b.tag_path]))

func compare_a_b_inheret() -> void:
	var matched := tag_a.match_inheritance(tag_b)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
	
	print_rich("[b]{1}[/color][/b]: Between the inheritance of [b]'{0}'[/b] and [b]'{2}'[/b]"
		.format([tag_a.tag_path, match_text, tag_b.tag_path]))
