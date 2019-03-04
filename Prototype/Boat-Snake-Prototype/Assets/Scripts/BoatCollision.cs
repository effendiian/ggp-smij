using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoatCollision : MonoBehaviour
{
	Boat boatScript;
	bool firstCollision = true;

    // Start is called before the first frame update
    void Start()
    {
		boatScript = GetComponentInParent<Boat>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

	//Detect collisions with the head
	private void OnTriggerEnter(Collider other)
    {
        if (other.transform.tag == "Swimmer")
        {
            boatScript.Collision(CollisionTypes.Swimmer, other);
        }
        else if (other.transform.tag == "Dock")
        {
            boatScript.Collision(CollisionTypes.Dock, other);
        }
        else if (other.transform.tag == "Node")
		{
			//The first collision is always the first node at the start of the game.
			//	Make sure we don't die then
			if (firstCollision)
			{
				firstCollision = false;
				return;
			}

			boatScript.Collision(CollisionTypes.Node, other);
		}
		else if (other.transform.tag == "Wall")
		{
			boatScript.Collision(CollisionTypes.Wall, other);
		}

	}
}
