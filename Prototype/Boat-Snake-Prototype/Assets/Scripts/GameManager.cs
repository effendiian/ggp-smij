using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : Singleton<GameManager>
{
    //Public fields
    public int weightPerNode = 5;
    public Transform entities;
    public Transform boundsPlane;
    public GameObject swimmerPrefab;
    public Collider[] obstacles;

    //Private fields
    float[] bounds;

    //Properties
    int Score { get; set; }

    // Start is called before the first frame update
    void Start()
    {
        //Create bounds
        bounds = new float[4];
        bounds[0] = boundsPlane.GetComponent<Collider>().bounds.max.z; //top
        bounds[1] = boundsPlane.GetComponent<Collider>().bounds.max.x; //right
        bounds[2] = boundsPlane.GetComponent<Collider>().bounds.min.z; //bottom
        bounds[3] = boundsPlane.GetComponent<Collider>().bounds.min.x; //left

        CreateSwimmer();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    /// <summary>
    /// Add to the total score
    /// </summary>
    /// <param name="tailLength">Length of the tail</param>
    public void AddToScore(int tailLength)
    {
        Score += (int)(weightPerNode * Mathf.Pow(tailLength, 2));
    }

    /// <summary>
    /// Create a random swimmer
    /// </summary>
    public void CreateSwimmer()
    {
        //Create the swimmer
        Transform swimmer = Instantiate(swimmerPrefab, entities).transform;
        float x = Random.Range(bounds[3], bounds[1]);
        float z = Random.Range(bounds[0], bounds[2]);
        swimmer.position = new Vector3(x, 0, z);

        while (true)
        {
            //Check if the new swimmer intersects with any of the obstacles
            bool hit = false;
            for(int i = 0; i < obstacles.Length; i++)
            {
                
                if (!Physics.CheckSphere(swimmer.position, 1))
                {
                    Debug.Log("intersecting");
                    hit = true;
                }
            }

            //Reset position
            if(hit)
            {
                x = Random.Range(bounds[3], bounds[1]);
                z = Random.Range(bounds[0], bounds[2]);
                swimmer.position = new Vector3(x, 0, z);
            }
            else break;
        }

    }
}
