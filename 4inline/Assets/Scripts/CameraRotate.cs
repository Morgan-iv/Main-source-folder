using UnityEngine;
using System.Collections;

public class CameraRotate : MonoBehaviour
{
	public Transform Target;
	public Camera goCamera;
	public float Distance = 5.0f;
	public float xSpeed = 250.0f;
	public float ySpeed = 120.0f;
	public float yMinLimit = -20.0f;
	public float yMaxLimit = 80.0f;
	private Vector3 StartPosition;
	private Quaternion StartRotation;
	private float x;
	private float y;
	void Awake()
	{
		Vector3 angles = goCamera.transform.eulerAngles;
		x = angles.x;
		y = angles.y;
		x += (float)(Input.GetAxis("Mouse X") * xSpeed * 0.02f);
		y -= (float)(Input.GetAxis("Mouse Y") * ySpeed * 0.02f);
		y = ClampAngle(y, yMinLimit, yMaxLimit);
		Quaternion rotation = Quaternion.Euler(y, x, 0);
		Vector3 position = rotation * (new Vector3(0.0f, 0.0f, -Distance)) + Target.position;
		transform.rotation = rotation;
		transform.position = position;
		StartPosition = GetComponent<Camera>().transform.position;
		StartRotation = GetComponent<Camera>().transform.rotation;
    }
	void LateUpdate()
	{
		if((Target != null)&&Input.GetMouseButton(1))
		{
			x += (float)(Input.GetAxis("Mouse X") * xSpeed * 0.02f);
			y -= (float)(Input.GetAxis("Mouse Y") * ySpeed * 0.02f);
			y = ClampAngle(y, yMinLimit, yMaxLimit);
			Quaternion rotation = Quaternion.Euler(y, x, 0);
			Vector3 position = rotation * (new Vector3(0.0f, 0.0f, -Distance)) + Target.position;
			transform.rotation = rotation;
			transform.position = position;
		}
		
	}
	public void CameraReturn()
	{
		goCamera.transform.position = StartPosition;
		goCamera.transform.rotation = StartRotation;
	}
	private float ClampAngle(float angle, float min, float max)
	{
		if(angle < -360)
			angle += 360;
		if(angle > 360)
			angle -= 360;
		return Mathf.Clamp (angle, min, max);
	}
}