import cv2

def draw_landmarks(img,pts):
    for i in range(len(pts)):
        nums = [0, 17, 22, 27, 31, 36, 42, 48, 60, 68]
        COLOR = (0,0,255,100)
        TICKNESS = 2

        for ind in range(len(nums) - 1):
            l, r = nums[ind], nums[ind + 1]
            targetX = pts[i][0, l:r]
            targetY = pts[i][1, l:r]
            for p in range(len(targetX)):
                cv2.circle(img,center=(targetX[p],targetY[p] ), color=COLOR, thickness=TICKNESS,radius=1)
