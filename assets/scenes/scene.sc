{
	"name": "MyScene",
	"objects": [
		{
			"name": "Object_0",
			"position": { "x": 2, "y": 2, "z": 2 },
			"rotation": { "x": 0, "y": 0, "z": 0, "w": 1 },
			"scale": { "x": 1, "y": 1, "z": 1 },
			"components": [
				{
					"type": "MeshComponent",
					"material": "materials/brick.mat",
					"mesh": {
						"type": "box",
						"x": 2,
						"y": 1,
						"z": 5
					}
				}

			]
		},
		{
			"name": "MainPlayer",
			"type": "Player",
			"position": { "x": 0, "y": 2, "z": 7 },
			"components": [
				{ "type": "CameraComponent" },
				{ "type": "PlayerControllerComponent" }
			],
			"children": [
				{
					"name": "Gun",
					"type": "gltf",
					"path": "models/weapons/scene.gltf",
					"position": { "x": 0.75, "y": -0.50, "z": -0.75 },
					"scale": { "x": -1, "y": 1, "z": 1 }
				}
			]
		}
	]

}