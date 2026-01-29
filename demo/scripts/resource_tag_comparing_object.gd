@tool
extends Node

@export var res_a : TagResource
@export var res_b : TagResource

@export_category("Actions")

@export_tool_button("Compare A and B") 
var compare_a_b_action : Callable = compare_a_b

@export_tool_button("Compare A's inheritance with B") 
var compare_a_b_inheret_action : Callable = compare_a_b_inheret

func compare_a_b() -> void:
	var matched := res_a.tag.match(res_b.tag)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
	
	print_rich("[b]{1}[/color][/b]: Between [b]'{0}'[/b] and [b]'{2}'[/b]!"
		.format([res_a.tag.get_tag_path(), match_text, res_b.tag.get_tag_path()]))

func compare_a_b_inheret() -> void:
	var matched := res_a.tag.match_inheritance(res_b.tag)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
	
	print_rich("[b]{1}[/color][/b]: Between the inheritance of [b]'{0}'[/b] and [b]'{2}'[/b]"
		.format([res_a.tag.get_tag_path(), match_text, res_b.tag.get_tag_path()]))
