extends Node

@export var tag_a : Tag
@export var tag_b : Tag
@export var tag_c : Tag

func _ready() -> void:
	print(tag_a.match(tag_b))
	print(tag_a.match(tag_c))
	
	print("mental")
