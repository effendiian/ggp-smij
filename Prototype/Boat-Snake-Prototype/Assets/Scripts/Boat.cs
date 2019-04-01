using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CollisionTypes {Swimmer, Node, Wall, Dock}

public class Boat : MonoBehaviour
{
	//Public fields
	public float speed = 5;
	public float turnSpeed = 100;
	public float minDistance = 0.5f;
	public GameObject nodePrefab;

    //Private fields
    bool crashed;
	List<Transform> nodes;

	List<Vector3> path;
	float timer;

    // Start is called before the first frame update
    void Start()
    {
        crashed = false;
		nodes = new List<Transform>();
		nodes.Add(transform.Find("Boat"));
		CreateNode();

		timer = 0;
		path = new List<Vector3>();
    }

    // Update is called once per frame
    void Update()
    {
		if (crashed)
			return;

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

		//DEBUG: Create a node with the space key
		if (Input.GetKeyDown(KeyCode.Space))
		{
			CreateNode();
		}
    }

	/// <summary>
	/// Create a node at the end of the trail
	/// </summary>
	void CreateNode()
	{
		Transform newNode = Instantiate(nodePrefab, nodes[nodes.Count - 1].position, nodes[nodes.Count - 1].rotation).transform;
		newNode.GetComponent<BoatFollower>().leader = nodes[nodes.Count - 1];
		newNode.SetParent(transform);
		nodes.Add(newNode);
	}

    /// <summary>
    /// Resolve a collision of the given type
    /// </summary>
    /// <param name="collisionType">The type of collision</param>
    /// <param name="col">The collider of the object's collision</param>
    public void Collision(CollisionTypes collisionType, Collider col)
	{
        if (crashed)
            return;

		switch (collisionType)
		{
            case CollisionTypes.Swimmer:
                CollisionSwimmer(col);
                break;
            case CollisionTypes.Dock:
                CollisionDock();
                break;
            case CollisionTypes.Node:
                Crash();
				break;
			case CollisionTypes.Wall:
                Crash();
				break;
		}
	}

    /// <summary>
    /// When the boat collides with a swimmer
    /// </summary>
    /// <param name="col">The collider of the object's collision</param>
    void CollisionSwimmer(Collider col)
    {
        CreateNode();
        Destroy(col.gameObject);
        GameManager.Instance.CreateSwimmer();
    }

    /// <summary>
    /// When the boat collides with the dock
    /// </summary>
    void CollisionDock()
    {
        //Add to the score
        GameManager.Instance.AddToScore(nodes.Count);

        //Remove tail
        for(int i = nodes.Count - 1; i > 1; i--)
        {
            Destroy(nodes[i].gameObject);
            nodes.RemoveAt(i);
        }
    }

    void Crash()
    {
        crashed = true;
    }
}
