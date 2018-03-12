var target : Transform; // Inspector> Assign the LookAt Camera Target Object
var distance = 10.0;    // distance of the camera from the Target Object
 
var xSpeed = 250.0; // Speed of x rotation
var ySpeed = 120.0; // Speed of y rotation
 
var yMinLimit = -20; // y minimum rotation limit
var yMaxLimit = 80;  // y maximum rotation limit
 
private var x = 0.0;
private var y = 0.0;
 
private var smooth = 0.0;
 
function Start () {
    var angles = transform.eulerAngles;
    x = angles.y;
    y = angles.x;
 
    // Make the rigid body not change rotation
    if (GetComponent.<Rigidbody>())
        GetComponent.<Rigidbody>().freezeRotation = true;
}
 
function LateUpdate () {
    if (target) {
        if(Input.GetMouseButton(0)) {
            x += Input.GetAxis("Mouse X") * xSpeed * 0.02;
            y -= Input.GetAxis("Mouse Y") * ySpeed * 0.02;
        }
         
        y = ClampAngle(y, yMinLimit, yMaxLimit);
                
        var rotation = Quaternion.Euler(y, x, 0);
        var position = rotation * Vector3(0.0, 0.5, -distance) + target.position;
         
        transform.rotation = rotation;
        transform.position = position;
    }
     
    if(Input.GetAxis("Mouse ScrollWheel"))  {
        smooth += Input.GetAxis("Mouse ScrollWheel");
    }
    distance += smooth;
    if(distance < 1)  // la Camera non si avvicina più di 1 unità
        distance = 1; 
    if(distance > 6) // la Camera non si allontana più di 6 unità
        distance = 6; 
    if(smooth != 0)
        smooth /= 1.2;
 
}
 
static function ClampAngle (angle : float, min : float, max : float) {
    if (angle < -360)
        angle += 360;
    if (angle > 360)
        angle -= 360;
    return Mathf.Clamp (angle, min, max);
}