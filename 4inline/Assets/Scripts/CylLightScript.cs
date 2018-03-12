using UnityEngine;
using System.Collections;

public class CylLightScript : MonoBehaviour {

	public GameObject MainScriptObject;
	public GameObject Cylinder;
	public Material BaseMat;
	public Material LightMat;
	
	void Start()
	{
	
	}
	void OnMouseEnter()
	{
		Cylinder.GetComponent<Renderer>().material = LightMat;
	}
	void Update()
	{
		if (Cylinder==MainScriptObject.GetComponent<MainScript>().LastCyl)
		{
			if ((int)(((Cylinder.transform.position.x+5)*5)+((Cylinder.transform.position.x+5)*0.5))==MainScriptObject.GetComponent<MainScript>().LastLeftCyl)
			{
				MainScriptObject.GetComponent<MainScript>().LastLeftCyl = 0;
				return;
			}
			Vector3 PosNew;
			GameObject CylNew;
			if (Input.GetKeyDown(KeyCode.LeftArrow))
			{
				PosNew = new Vector3(0, 0, -2);
				Collider[] col = Physics.OverlapSphere(Cylinder.transform.position+PosNew, (float)0.1);
				if (col.Length>0)
				{
					CylNew = col[0].gameObject;
					CylNew.GetComponent<Renderer>().material = LightMat;
					MainScriptObject.GetComponent<MainScript>().LastCyl = CylNew;
					Cylinder.GetComponent<Renderer>().material = BaseMat;
					MainScriptObject.GetComponent<MainScript>().LastLeftCyl = (int)(((CylNew.transform.position.x+5)*5)+((CylNew.transform.position.x+5)*0.5));
				}
				return;
			}
			if (Input.GetKeyDown(KeyCode.RightArrow))
			{
				PosNew = new Vector3(0, 0, 2);
				Collider[] col = Physics.OverlapSphere(Cylinder.transform.position+PosNew, (float)0.1);
				if (col.Length>0)
				{
					CylNew = col[0].gameObject;
					CylNew.GetComponent<Renderer>().material = LightMat;
					MainScriptObject.GetComponent<MainScript>().LastCyl = CylNew;
					Cylinder.GetComponent<Renderer>().material = BaseMat;
					MainScriptObject.GetComponent<MainScript>().LastLeftCyl = (int)(((CylNew.transform.position.x+5)*5)+((CylNew.transform.position.x+5)*0.5));
				}
				return;
			}
			if (Input.GetKeyDown(KeyCode.UpArrow))
			{
				PosNew = new Vector3(-2, 0, 0);
				Collider[] col = Physics.OverlapSphere(Cylinder.transform.position+PosNew, (float)0.1);
				if (col.Length>0)
				{
					CylNew = col[0].gameObject;
					CylNew.GetComponent<Renderer>().material = LightMat;
					MainScriptObject.GetComponent<MainScript>().LastCyl = CylNew;
					Cylinder.GetComponent<Renderer>().material = BaseMat;
					MainScriptObject.GetComponent<MainScript>().LastLeftCyl = (int)(((CylNew.transform.position.x+5)*5)+((CylNew.transform.position.x+5)*0.5));
				}
				return;
			}
			if (Input.GetKeyDown(KeyCode.DownArrow))
			{
				PosNew = new Vector3(2, 0, 0);
				Collider[] col = Physics.OverlapSphere(Cylinder.transform.position+PosNew, (float)0.1);
				if (col.Length>0)
				{
					CylNew = col[0].gameObject;
					CylNew.GetComponent<Renderer>().material = LightMat;
					MainScriptObject.GetComponent<MainScript>().LastCyl = CylNew;
					Cylinder.GetComponent<Renderer>().material = BaseMat;
					MainScriptObject.GetComponent<MainScript>().LastLeftCyl = (int)(((CylNew.transform.position.x+5)*5)+((CylNew.transform.position.x+5)*0.5));
				}
				return;
			}
		}
	}
	void OnMouseDown()
	{
		if (MainScriptObject.GetComponent<MainScript>().LastCyl != null)
		    MainScriptObject.GetComponent<MainScript>().LastCyl.GetComponent<Renderer>().material = BaseMat;
		MainScriptObject.GetComponent<MainScript>().LastCyl = Cylinder;
		Cylinder.GetComponent<Renderer>().material = LightMat;
		if (!MainScriptObject.GetComponent<MainScript>().ClickNeeded)
		{
			MainScriptObject.GetComponent<MainScript>().HumanTurn();
		}
	}

	void OnMouseExit()
	{
		if (MainScriptObject.GetComponent<MainScript>().LastCyl != Cylinder)
			Cylinder.GetComponent<Renderer>().material = BaseMat;
	}
}
