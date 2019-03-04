using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CollisionTypes {Node, Wall}

public class Boat : MonoBehaviour
{
	//Public fields
	public float speed = 5;
	public float turnSpeed = 100;
	public float minDistance = 0.5f;
	public GameObject nodePrefab;

	//Private fields
	List<Transform> nodes;

    // Start is called before the first frame update
    void Start()
    {
		nodes = new List<Transform>();
		nodes.Add(transform.Find("Boat"));
		CreateNode();
    }

    // Update is called once per frame
    void Update()
    {
		Move();

		//DEBUG: Create a node with the space key
		if (Input.GetKeyDown(KeyCode.Space))
		{
			CreateNode();
		}
    }

	/// <summary>
	/// Move the snake
	/// </summary>
	void Move()
	{
		//Rotate and translate boat
		if (Input.GetAxisRaw("Horizontal") != 0)
		{
			nodes[0].Rotate(0, turnSpeed * Time.deltaTime * Input.GetAxisRaw("Horizontal"), 0);
		}
		if (Input.GetAxisRaw("Vertical") != 0)
		{
			nodes[0].Rotate(0, turnSpeed * Time.deltaTime * Input.GetAxisRaw("Vertical"), 0);
		}
		nodes[0].Translate(nodes[0].forward * speed * Time.deltaTime, Space.World);

		//Rotate and translate nodes
		Transform currNode = null;
		Transform prevNode = null;
		for(int i = 1; i < nodes.Count; i++)
		{
			currNode = nodes[i];
			prevNode = nodes[i - 1];

			float dis = Vector3.Distance(prevNode.position, currNode.position);
			Vector3 newPos = prevNode.position;

			//Calculate and apply slerps
			float t = Time.deltaTime * dis / minDistance * speed;
			if (t > 0.5f)
				t = 0.5f;
			currNode.position = Vector3.Lerp(currNode.position, newPos, t);
			currNode.rotation = Quaternion.Lerp(currNode.rotation, prevNode.rotation, t);
		}
	}

	/// <summary>
	/// Create a node at the end of the trail
	/// </summary>
	void CreateNode()
	{
		Transform newNode = Instantiate(nodePrefab, nodes[nodes.Count - 1].position, nodes[nodes.Count - 1].rotation).transform;
		newNode.SetParent(transform);
		nodes.Add(newNode);
	}

	/// <summary>
	/// Resolve a collision of the given type
	/// </summary>
	/// <param name="collisionType">The type of collision</param>
	public void Collision(CollisionTypes collisionType)
	{
		switch (collisionType)
		{
			case CollisionTypes.Node:
				Debug.Log("Collision with node!");
				break;
			case CollisionTypes.Wall:
				Debug.Log("Collision with wall!");
				break;
		}
	}
}
