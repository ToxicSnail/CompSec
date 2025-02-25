#include <iostream>
#include <vector>
#include <ostream>

class Image 
{
public:
    int width, height;
    
    Image(int w, int h) : width(w), height(h) {}
};

class Url 
{
public:
    std::string link;
    
    Url(const std::string& url) : link(url) {}
};

class Account 
{
public:
    std::string username;
    
    Account(const std::string& name) : username(name) {}
};

class ImageStorage 
{
public:
    static void Save(const Image& image, const std::string& path) 
    {
        std::cout << "Saving image to: " << path << std::endl;
    }

    static int DeleteDuplicates() 
    {
        std::cout << "Deleting duplicate images..." << std::endl;
        return 2;
    }
};

class ImageProcessor 
{
public:
    static Image Resize(const Image& image, int height, int width) 
    {
        std::cout << "Resizing image to " << width << "x" << height << std::endl;
        return Image(width, height);
    }

    static Image InvertColors(const Image& image) 
    {
        std::cout << "Inverting colors of the image..." << std::endl;
        return image;
    }
};

class ImageDownloader 
{
public:
    static Image Download(const Url& imageUrl) 
    {
        std::cout << "Downloading image from: " << imageUrl.link << std::endl;
        return Image(800, 600);
    }
};

class AccountService 
{
public:
    static void SetImageAsAccountPicture(const Image& image, const Account& account) 
    {
        std::cout << "Setting image as profile picture for user: " << account.username << std::endl;
    }
};

int main()
{
    ImageStorage storage;
    AccountService accountService;
    
    Url imageUrl("https://example.com/hehe/image.jpg");
    Image downloadedImage = ImageDownloader::Download(imageUrl);

    Image resizedImage = ImageProcessor::Resize(downloadedImage, 300, 300);
    Image invertedImage = ImageProcessor::InvertColors(resizedImage);

    storage.Save(invertedImage, "C:/path/vovo/hehehe.jpg");

    int duplicatesRemoved = storage.DeleteDuplicates();
    std::cout << "Removed " << duplicatesRemoved << " duplicate images." << std::endl;

    Account user("JohnDoe");
    accountService.SetImageAsAccountPicture(invertedImage, user);

    return 0;
}
