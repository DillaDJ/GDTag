@tool
extends Node

@export var container_a : TagContainer
@export var container_b : TagContainer

@export_tool_button("Check Containers Share Any Tag") 
var check_any_button : Callable = check_any

@export_tool_button("Check Containers Exact Match") 
var check_exact_button : Callable = check_exact

@export_tool_button("Check A has all Tags in B") 
var check_all_button : Callable = check_all

@export_tool_button("Check A has no Tags in B") 
var check_none_button : Callable = check_none

@export_tool_button("Print Shared Tags") 
var print_overlaps_button : Callable = print_overlaps


func check_any() -> void:
	var matched := container_a.any(container_b)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
		
	var has_text : String = ("At least one tag is shared" 
		if matched == true else "No tags are shared")
	
	print_rich("[b]{0}[/color][/b]: {1} between the containers!"
		.format([match_text, has_text]))


func check_exact() -> void:
	var matched := container_a.exact(container_b)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
		
	var has_text : String = ("Exact match" if matched == true else "No exact match")
	
	print_rich("[b]{0}[/color][/b]: {1} between the containers!"
		.format([match_text, has_text]))


func check_all() -> void:
	var matched := container_a.all(container_b)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
		
	var has_text : String = ("has" if matched == true else "does not have")
	
	print_rich("[b]{0}[/color][/b]: [b]ContainerA[/b] {1} all of the tags in [b]ContainerB[/b]!"
		.format([match_text, has_text]))


func check_none() -> void:
	var matched := container_a.none(container_b)
	
	var match_text : String = ("[color=green]Match" if matched == true 
		else "[color=red]No Match")
		
	var has_text : String = ("No tags are shared" 
		if matched == true else "At least one tag is shared")
	
	print_rich("[b]{0}[/color][/b]: {1} between the containers!"
		.format([match_text, has_text]))


func print_overlaps() -> void:
	var overlaps := container_a.overlap_count(container_b)
	print_rich("The containers have [b]{0}[/b] shared tag(s)!".format([overlaps]))
