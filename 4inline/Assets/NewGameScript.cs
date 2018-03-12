using UnityEngine;
using System.Collections;

public class NewGameScript : MonoBehaviour {

	public GameObject MainScriptObject;
	
	// Use this for initialization
	void Start ()
	{
		MainScriptObject.GetComponent<MainScript>().NewGame(false);
	}
	
	// Update is called once per frame
	void Update () {
		
	}
	
	void OnGUI(){
		if (GUI.Button(new Rect (25, 25, 100, 30), "Одиночная игра")){
   			Debug.Log("dsdasdadasda");
  		}
		else if (GUI.Button(new Rect (125, 125, 100, 30), "Выход")){
    		Debug.Log("hfghhf");
  		}
}
}
