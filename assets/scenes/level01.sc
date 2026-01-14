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
			],
			"children": [
				{
					"name": "Gun",
					"type": "gltf",
					"path": "models/weapons/scene.gltf",
					"position": {
						"x": 0.75,
						"y": -0.5,
						"z": -0.75
					},
					"scale": {
						"x": -1.0,
						"y": 1.0,
						"z": 1.0
					}
				},
				{
					"name": "Cubedude",
					"type": "gltf",
					"path": "models/cube.gltf",
					"position": {
						"x": -2,
						"y": 1,
						"z": -2
					},
					"scale": {
						"x": 0.1,
						"y": 0.11,
						"z": 0.11
					}
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
			"name": "RightWall",
			"position": {
				"x": 15,
				"y": 5,
				"z": 0
			},
			"components": [
				{
					"type": "MeshComponent",
					"mesh": {
						"type": "box",
						"x": 1,
						"y": 10,
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
						"x": 1,
						"y": 10,
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
			"name": "LeftWall",
			"position": {
				"x": -15,
				"y": 5,
				"z": 0
			},
			"components": [
				{
					"type": "MeshComponent",
					"mesh": {
						"type": "box",
						"x": 1,
						"y": 10,
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
						"x": 1,
						"y": 10,
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
			"name": "FrontWall",
			"position": {
				"x": 0,
				"y": 5,
				"z": -15
			},
			"components": [
				{
					"type": "MeshComponent",
					"mesh": {
						"type": "box",
						"x": 30,
						"y": 10,
						"z": 1
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
						"y": 10,
						"z": 1
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
			"name": "BackWall",
			"position": {
				"x": 0,
				"y": 5,
				"z": 15
			},
			"components": [
				{
					"type": "MeshComponent",
					"mesh": {
						"type": "box",
						"x": 30,
						"y": 10,
						"z": 1
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
						"y": 10,
						"z": 1
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
			"name": "Roof",
			"position": {
				"x": 0,
				"y": 10,
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
			"name": "Sphere01",
			"position": {
				"x": 10,
				"y": 2,
				"z": 10
			},
			"components": [
				{
					"type": "MeshComponent",
					"material": {
						"path": "materials/checker.mat",
						"params": {
							"float3": [
								{
									"name": "colour",
									"value0": 1.0,
									"value1": 0.0,
									"value2": 1.0
								}
							]
						}
					},
					"mesh": {
						"type": "sphere",
						"radius": 2
					}
				},
				{
					"type": "PhysicsComponent",
					"collider": {
						"type": "sphere",
						"radius": 2
					},
					"body": {
						"mass": 2,
						"friction": 0.5,
						"type": "dynamic"
					}
				}
			]
		},
		{
			"name": "Cube01",
			"objType": "destructable",
			"position": {
				"x": 4,
				"y": 2,
				"z": 4
			},
			"components": [
				{
					"type": "MeshComponent",
					"material": {
						"path": "materials/checker.mat",
						"params": {
							"float3": [
								{
									"name": "colour",
									"value0": 1.0,
									"value1": 0.0,
									"value2": 0.0
								}
							]
						}
					},
					"mesh": {
						"type": "box",
						"x": 2,
						"y": 2,
						"z": 2
					}
				},
				{
					"type": "PhysicsComponent",
					"collider": {
						"type": "box",
						"x": 2,
						"y": 2,
						"z": 2
					},
					"body": {
						"mass": 2,
						"friction": 0.5,
						"type": "dynamic"
					}
				}
			]
		},
		{
			"name": "JumpPlatform",
			"type": "JumpPlatform",
			"position": {
				"x": -7,
				"y": 1.2,
				"z": 1
			},
			"components": [
				{
					"type": "MeshComponent",
					"mesh": {
						"type": "box",
						"x": 2,
						"y": 0.2,
						"z": 2
					},
					"material": {
						"path": "materials/checker.mat",
						"params": {
							"float3": [
								{
									"name": "colour",
									"value0": 1.0,
									"value1": 0.0,
									"value2": 0.0
								}
							]
						}
					}
				},
				{
					"type": "AudioComponent",
					"audio": [
						{
							"name": "boing",
							"path": "audio/boing.wav",
							"volume": 10
						}
					]
				},
				{
					"type": "PhysicsComponent",
					"collider": {
						"type": "box",
						"x": 2,
						"y": 0.2,
						"z": 2
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
			"name": "Sphere02",
			"position": {
				"x": 5,
				"y": 2,
				"z": 10
			},
			"components": [
				{
					"type": "MeshComponent",
					"material": {
						"path": "materials/checker.mat",
						"params": {
							"float3": [
								{
									"name": "colour",
									"value0": 0.0,
									"value1": 0.0,
									"value2": 1.0
								}
							]
						}
					},
					"mesh": {
						"type": "sphere",
						"radius": 1
					}
				},
				{
					"type": "PhysicsComponent",
					"collider": {
						"type": "sphere",
						"radius": 1
					},
					"body": {
						"mass": 2,
						"friction": 0.5,
						"type": "dynamic"
					}
				}
			]
		},
		{
			"name": "Sphere03",
			"objType": "destructable",
			"position": {
				"x": -5,
				"y": 2,
				"z": 10
			},
			"components": [
				{
					"type": "MeshComponent",
					"material": {
						"path": "materials/checker.mat",
						"params": {
							"float3": [
								{
									"name": "colour",
									"value0": 0.0,
									"value1": 1.0,
									"value2": 1.0
								}
							]
						}
					},
					"mesh": {
						"type": "sphere",
						"radius": 1
					}
				},
				{
					"type": "PhysicsComponent",
					"collider": {
						"type": "sphere",
						"radius": 1
					},
					"body": {
						"mass": 2,
						"friction": 0.5,
						"type": "dynamic"
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