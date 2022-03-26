import numpy as np
import cv2
import scipy
from scipy.signal import convolve2d
import scipy.io
import imageio
import skimage as sk
from skimage.measure import compare_psnr
import matplotlib.pyplot as plt

import warnings
warnings.filterwarnings('ignore')

DEFAULT_KERNEL_SIZE = 20
IMAGE_SIZE = 256
IMAGE_CENTER = (IMAGE_SIZE-1) // 2
hundred_motion_paths='100_motion_paths.mat'
default_path_to_image='DIPSourceHW1.jpg'
WHITE_LEVEL = 255

def determine_motion_values(kernel, x_motion, y_motion,kernel_center):
            x_location = int(round(kernel_center + ((x_motion * (IMAGE_SIZE/DEFAULT_KERNEL_SIZE)) / DEFAULT_KERNEL_SIZE) ))
            y_location = int(round(kernel_center + ((y_motion * (IMAGE_SIZE/DEFAULT_KERNEL_SIZE)) / DEFAULT_KERNEL_SIZE) ))
            x_location = max(0, min(DEFAULT_KERNEL_SIZE - 1,x_location))
            y_location = max(0, min(DEFAULT_KERNEL_SIZE - 1, y_location))
            if y_location>= DEFAULT_KERNEL_SIZE - 1:
                y_location= DEFAULT_KERNEL_SIZE - 1
            kernel[x_location][y_location] += 1
			
class Camera_Motion:
    def __init__(self, motion_path: str=hundred_motion_paths):
        matrix=scipy.io.loadmat(motion_path)
        self.X=matrix['X']
        self.Y=matrix['Y']
        self.number_of_motions=len(self.X)
    
    def get_Motion (self,motion_file:int)->(np.ndarray, np.ndarray):
        return self.X[motion_file], self.Y[motion_file]
    
    def save_Motion(self, motion_file:int):
        X , Y = self.get_Motion(motion_file)
        plt.plot(X,Y)
        plt.xlabel('Motion along the X-Axis')
        plt.ylabel('Motion along the Y-Axis')
        plt.title("Camera Motion for File = {}".format(motion_file))
        plt.savefig("100_Camera_Motions/Camera_Motion_{}".format(motion_file))
        plt.close()
    
    def save_Motions(self):
        range_limit=len(self.X)
        for i in range (0,range_limit):
            self.save_Motion(i)

    def PSF_Kernel(self,motion_file:int)->np.ndarray:
        psf_kernel=np.zeros((DEFAULT_KERNEL_SIZE,DEFAULT_KERNEL_SIZE),int)
        psf_kernel_center=(DEFAULT_KERNEL_SIZE-1)//2
        X, Y = self.get_Motion(motion_file)
        for x_motion,y_motion in zip(X,Y):
            determine_motion_values(psf_kernel, x_motion, y_motion,psf_kernel_center)
        return np.rot90(psf_kernel)
            
    def save_PSFS(self):
        PSF_Kernels = []
        for i in range(self.number_of_motions):
            PSF_Kernels.append(self.PSF_Kernel(i))
            imageio.imwrite("100_PSFS/PSF_{}.jpg".format(i), sk.util.img_as_ubyte(PSF_Kernels[i]))

def get_Original_Image(path_to_image):
    image=cv2.imread(path_to_image, cv2.IMREAD_GRAYSCALE).astype(float)
    image /= WHITE_LEVEL
    image = cv2.resize(image, (IMAGE_SIZE, IMAGE_SIZE,))
    return image

class Blurred_Image:
    def __init__(self, PSF_Kernel,path_to_image=default_path_to_image):
        self.original_image=get_Original_Image(path_to_image)
        self.blurred_image=convolve2d(self.original_image, PSF_Kernel,mode='same', boundary='wrap') 
        
def get_Blurred_Images(blurred_images,num_of_images,motions,path_to_image):
    for i in range(num_of_images):
        blurred_image = Blurred_Image(motions.PSF_Kernel(i),path_to_image)
        blurred_images.append(blurred_image.blurred_image)
    
def save_Blurred_Images (blurred_images,number_of_blurred_images):
    for i in range (number_of_blurred_images):
        imageio.imwrite("100_BLURRED_IMAGES/Blurred_Image_{}.jpg".format(i), blurred_images[i])
        
		
class Blurred_Images:   
    def __init__(self, path_to_image=default_path_to_image, path_to_motions=hundred_motion_paths):
        self.motions=Camera_Motion(path_to_motions)
        self.number_of_images = self.motions.number_of_motions
        self.blurred_images = []
        get_Blurred_Images(self.blurred_images,self.number_of_images,self.motions,path_to_image)
        save_Blurred_Images(self.blurred_images,self.number_of_images)
		
def update_Fourier_List(index,blurred_images,DEFAULT_POWER):
    fourier_list=[]
    v_list=[]
    v_sum=0
    for i in range(index):
        fft=np.fft.fftshift(np.fft.fft2(blurred_images[i]))
        fourier_list.append(fft)
        v=np.abs(fft)**DEFAULT_POWER
        v_list.append(v)
        v_sum+=v
    return fourier_list,v_list,v_sum

def deblurring_Using_Weighted_Fourier_Burst_Accumulation(number_of_images,blurred_images,DEFAULT_POWER):
    deblurred_images=[]
    for i in range(1, number_of_images + 1):
            weights_list = []
            weight_of_image = 0
            fourier_list,v_list,v_sum=update_Fourier_List(i,blurred_images,DEFAULT_POWER)
            for j in range(i):
                weights_list.append(v_list[j] / v_sum)
            for j in range(i):
                weight_of_image += weights_list[j]*fourier_list[j]
            deblurred_image = np.abs(np.fft.ifft2(np.fft.ifftshift(weight_of_image)))
            deblurred_images.append(deblurred_image)
    return deblurred_images

def saving_Deblurred_Images(deblurred_images,num_of_images):
    for i in range(num_of_images):
        imageio.imwrite("100_DEBLURRED_IMAGES/Deblurred_Image_{}.jpg".format(i), deblurred_images[i])

def get_PSNR_Values(deblurred_images,original_image):
    psnr_values=[]
    for i in deblurred_images:
        PSNR=abs(compare_psnr(original_image,i))
        psnr_values.append(PSNR)
    return psnr_values

def saving_PSNR_Values(psnr_values):
    plt.plot((np.linspace(1, len(psnr_values) , len(psnr_values))), psnr_values)
    plt.title("PSNR as a function of the Nr.Of Images Used to Deblur ")
    plt.xlabel('Nr.Of Images Used to Deblur')
    plt.ylabel('PSNR Values [dB]')
    plt.show()
    plt.close()
    plt.plot((np.linspace(1, len(psnr_values) , len(psnr_values))), psnr_values)
    plt.title("PSNR as a function of the Nr.Of Images Used to Deblur ")
    plt.xlabel('Nr.Of Images Used to Deblur')
    plt.ylabel('PSNR Values [dB]')
    plt.savefig("PSNR Graph")
    plt.close()

def choose_Optimal_Power(blurred_images,num_of_images,original_image):
        DEFAULT_POWER=20
        Optimal_PSNR_Average=0
        Optimal_Power=1
        for i in range(0,5):
            deblurred_images=deblurring_Using_Weighted_Fourier_Burst_Accumulation(num_of_images,blurred_images,DEFAULT_POWER)
            PSNR_values=get_PSNR_Values(deblurred_images,original_image)
            PSNR_Average=((np.sum(PSNR_values))/num_of_images)
            print("For power:")
            print(DEFAULT_POWER)
            print("PSNR_Average=")
            print(PSNR_Average)
            print("**************************")
            if Optimal_PSNR_Average<PSNR_Average:
                Optimal_Power=DEFAULT_POWER
                Optimal_PSNR_Average=PSNR_Average
            DEFAULT_POWER+=1
        return Optimal_Power

class Image_Deblurring:
    def __init__(self,blurred_images,path_to_original_image):
        self.num_of_images=len(blurred_images)
        self.blurred_images=blurred_images
        self.deblurred_images=[]
        self.PSNR_values=[]
        original_image=get_Original_Image(path_to_original_image)
        #DEFAULT_POWER=choose_Optimal_Power(self.blurred_images,self.num_of_images,original_image)
        DEFAULT_POWER=30
        #print("Optimal Power=")
        #print(DEFAULT_POWER)
        self.deblurred_images=deblurring_Using_Weighted_Fourier_Burst_Accumulation(self.num_of_images,blurred_images,DEFAULT_POWER)
        self.PSNR_values=get_PSNR_Values(self.deblurred_images,original_image)
        saving_Deblurred_Images(self.deblurred_images,self.num_of_images)
        saving_PSNR_Values(self.PSNR_values)
	
motion=Camera_Motion()
motion.save_Motions()
motion.save_PSFS()
blurred_images=Blurred_Images()
image_deblurring=Image_Deblurring(blurred_images.blurred_images,default_path_to_image)