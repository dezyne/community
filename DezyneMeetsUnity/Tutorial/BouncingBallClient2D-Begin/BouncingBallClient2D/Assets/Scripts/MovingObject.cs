using UnityEngine;

public class MovingObject : MonoBehaviour
{
    public bool moving = false;
    public float moveSpeed = 2;

    private void OnCollisionEnter(Collision collision)
    {
        DznMessageSender sender = GetComponent<DznMessageSender>();
        sender.SendNetworkMessage("WallHit");
    }
    
    void Update()
    {
        if (moving)
        {
            transform.position += transform.forward * moveSpeed * Time.deltaTime;
        }
    }
    
    public void StartMoving()
    {
        moving = true;
    }

    public void StopMoving()
    {
        moving = false;
    }

    public void Reverse()
    {
        transform.Rotate(transform.up, 180);
    }
}
