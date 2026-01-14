{
	"name": "MyScene",
	"objects": [
		{
			"name": "MainPlayer",
			"type": "Player",
			"position": {
				"x": 2,
				"y": 10,
				"z": 2
			},
			"components": [
				{ "type": "CameraComponent" },
				{ "type": "PlayerControllerComponent" },
				{ "type": "AudioListenerComponent" },
				{
					"type": "AudioComponent",
					"audio": [
						{
							"name": "shoot",
							"path": "audio/shoot.wav"
						},
						{
							"name": "step",
							"path": "audio/step.wav"
						},
						{
							"name": "jump",
							"path": "audio/jump.wav"
						}
					]
				}
			]
		},
		{
			"name": "Ground",
			"position": {
				"x": 0,
				"y": 0,
				"z": 0
			},
			"components": [
				{
					"type": "MeshComponent",
					"mesh": {
						"type": "box",
						"x": 30,
						"y": 1,
						"z": 30
					},
					"material": {
						"path": "materials/checker.mat"
					}

				},
				{
					"type": "PhysicsComponent",
					"collider": {
						"type": "box",
						"x": 30,
						"y": 1,
						"z": 30
					},
					"body": {
						"mass": 0,
						"friction": 0.5,
						"type": "static"
					}
				}
			]
		},
		{
			"name": "Light",
			"position": {
				"x": 0,
				"y": 10,
				"z": 0
			},
			"components": [
				{
					"type": "DiscoLight",
					"colour": {
						"r": 1,
						"g": 1,
						"b": 1
					}
				}
			]
		}
	],
	"camera" :  "MainPlayer"
}