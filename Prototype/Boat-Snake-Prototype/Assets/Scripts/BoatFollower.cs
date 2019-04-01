using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//https://gamedev.stackexchange.com/questions/120189/unity-method-to-get-a-vector-which-is-x-seconds-behind-another-moving-vector-a/120225#120225

public class BoatFollower : MonoBehaviour
{
	const int MAX_FPS = 60;

	public Transform leader;
	float lagSeconds = 0.3f;

	Vector3[] positionBuffer;
	float[] timeBuffer;
	int oldestIndex;
	int newestIndex;
	float timer;

	// Use this for initialization
	void Start()
	{
		int bufferLength = Mathf.CeilToInt(lagSeconds * MAX_FPS);
		positionBuffer = new Vector3[bufferLength];
		timeBuffer = new float[bufferLength];

		positionBuffer[0] = positionBuffer[1] = leader.position;
		timeBuffer[0] = timeBuffer[1] = Time.time;

		oldestIndex = 0;
		newestIndex = 1;
	}

	// Update is called once per frame
	void Update()
	{
		//timer += Time.deltaTime;
		//if (timer > lagSeconds)
		//{
		//	timer -= lagSeconds;

			// Insert newest position into our cache.
			// If the cache is full, overwrite the latest sample.
			int newIndex = (newestIndex + 1) % positionBuffer.Length;
			if (newIndex != oldestIndex)
				newestIndex = newIndex;

			positionBuffer[newestIndex] = leader.position;
			timeBuffer[newestIndex] = Time.time;
		//}

		// Skip ahead in the buffer to the segment containing our target time.
		float targetTime = Time.time - lagSeconds;
			int nextIndex;
			while (timeBuffer[nextIndex = (oldestIndex + 1) % timeBuffer.Length] < targetTime)
				oldestIndex = nextIndex;

		// Interpolate between the two samples on either side of our target time.
		float span = timeBuffer[nextIndex] - timeBuffer[oldestIndex];
		float progress = 0f;
		if (span > 0f)
		{
			progress = (targetTime - timeBuffer[oldestIndex]) / span;
		}

		transform.position = Vector3.Lerp(positionBuffer[oldestIndex], positionBuffer[nextIndex], progress);
	}
}
