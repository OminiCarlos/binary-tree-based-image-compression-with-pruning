# binary-tree-based-image-compression-with-pruning

This image compression project reduces the image size by converting an image into a binary tree and pruning the leaf nodes. The process begins by dividing the image into two smaller sections along a line where the total standard deviation of RGB values is minimized. The average RGB value, the range of the subimage are stored as a node. This node also have 2 pointers indicating the node representing its subimage. This splitting process is repeated recursively until each node in the binary tree represents a single pixel.

Once the binary tree is fully constructed, the pruning process is executed. To minimize the image size, the braches are pruned as high as possible. For each subtree, the subimage's average color is compared with each leaf node, which are pixels in that range. If a leaf node is close to the average color within a specified tolerance, it is considered valid. The algorithm counts vaild leaf nodes. If the percentage of valid leaf nodes is higher then a predetermined percentage, the entire subtree is pruned, and the subimage is replaced with an area with its average color.

Original image:
![kkkk-kmnn-960x540](https://github.com/user-attachments/assets/9d028206-57c2-4bb2-b558-7f3d0c35a4ac)

Compressed image:
![kkkk-kmnn-960x540-prune-pct85-tol002-r1](https://github.com/user-attachments/assets/12967395-79ff-4f2a-89af-efe35a763563)
