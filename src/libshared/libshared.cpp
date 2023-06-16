#include "libshared.hpp"

using namespace std;

//char mkey[] = "shmfile";
string path_val;
fstream in_file;


// int
// _create_and_write_shared_memory (char *mmap_key, const char *value, unsigned int size){
    
//     int shm_fd;
    
//     /*Create a shared memory object in kernel space. If shared memory already
//      * exists it will truncate it to zero bytes again*/
//     shm_fd = shm_open(mmap_key, O_CREAT | O_RDWR | O_TRUNC, 0660);
    
//     if (shm_fd < 0) {
//         printf("failure on shm_open on shm_fd, errcode = %d\n", errno);
//         return -1;
//     }

//     if (ftruncate(shm_fd, size) == -1) {
//         printf("Error on ftruncate to allocate size of shared memory region\n");
//         return -1;
//     }

//     /*Now map the shared memory in kernel space into process's Virtual address space*/
//     void *shm_reg = mmap(NULL, // let the kernel chose to return the base address of shm memory
//             size,              // sizeof the shared memory to map to virtual address space of the process
//             PROT_READ | PROT_WRITE, // shared memory is Read and Writable
//             MAP_SHARED,        // shared memory is accessible by different processes
//             shm_fd,            // file descriptor of the shared memory 
//             0);                // offset from the base address of the physical/shared memory to be mapped

//     /* shm_reg is the address in process's Virtual address space, just like any other address.
//      * The Linux paging mechanism maps this address to starting address of the shared memory region 
//      * in kernel space. Any operation performed by process on shm_reg address is actually the operation
//      * performed in shared memory which resides in kernel*/
//     memset(shm_reg, 0, size);
//     memcpy(shm_reg, value, size);
//     munmap(shm_reg, size);
//     /*Reader process will not be able to read shm if writer unlink
//      * the name below*/
//     //shm_unlink(mmap_key);
//     close(shm_fd);
//     return size;
// }

// int
// _read_from_shared_memory (char *mmap_key, 
//                          char *buffer, 
//                          //unsigned int buff_size, 
//                          unsigned int bytes_to_read) {


//     int shm_fd = 0, rc = 0;

//     shm_fd = shm_open(mmap_key, O_CREAT | O_RDONLY , 0660);
    
//     if (shm_fd < 0) {
//         printf("failure on shm_open on shm_fd, error code = %d", errno);
//         return -1;
//     }

//     void *shm_reg = mmap (NULL, bytes_to_read, PROT_READ, MAP_SHARED, shm_fd, 0);

//     if(shm_reg == MAP_FAILED){
//         printf("Error on mapping\n");
//         return -1;
//     }
    
//     memcpy(buffer, shm_reg, bytes_to_read);
//     rc = munmap(shm_reg, bytes_to_read);

//     if(rc < 0){
//         printf("munmap failed\n");
//         return -1;
//     }
//     cout << "buffer from reading shm: " << buffer << endl;
//     close(shm_fd);
//     //return bytes_to_read; /*Return the number of bytes read */
//     return strlen(buffer);
// }


// unsigned short _get_buffer(char* buffer)
// {
//     int read_size = _read_from_shared_memory (mkey, buffer, BUF_SIZE);
//     cout << buffer << endl;
//     if(read_size == -1) return 255;
//     if(read_size == 0) return 4;
    
//     return 0;

// }

unsigned short load_resource(string path)
{
    path_val = path;
    in_file.open(path, ios::out);
    if(!in_file.is_open()) 
    {
        //perror("Resource file open error ..."); 
        return 1;
    }
    in_file.close();

    return 0;
}

unsigned short get_value(const string& key, string& value)
{
    string line {};
    if(!path_val.empty())
    {
        in_file.open(path_val, fstream::in);
        if(!in_file.is_open()) 
        {
            //perror("Resource file open error ..."); 
            return 4;
        }
        while(getline(in_file, line))
        {
            if(line.find(key)!= string::npos)
            {
                value = line.substr(key.length() + 3, line.length() - key.length() - 3);
                //cout << "key is " << key << " value is " << value << endl;
                in_file.close();
                return 0;
            }
        }
        in_file.close();
        return 3;
    }
    //perror("No file available ...");
    return 255;

}

unsigned short set_value(const std::string& key, const std::string &value)
{
    string line {};
    
    if(!path_val.empty())
    {
        in_file.open(path_val);
        //cout << "File " << path_val << " opened correctly" << endl;
        if(!in_file.is_open()) 
        {
            //perror("Resource file open error ..."); 
            return 4;
        }

        /* Output the content of the file to a string */
        string text(
          (istreambuf_iterator<char>(in_file)),
          istreambuf_iterator<char>());

        in_file.close();
        size_t pos = 0;
        if ((pos = text.find(key, pos)) != string::npos) 
        {
                pos += key.length() + 3;
                size_t pos_break;
                if((pos_break = text.find("\n", pos)) == string::npos) 
                {
                    //cout << "Error reading file." << endl;
                    in_file.close();
                    return 255;
                }
                //cout << "Pos is " << pos << " and pos break is " << pos_break;
                text.replace(pos , pos_break - pos, value);
                //cout << text << endl;
                in_file.open(path_val, fstream::in | fstream::out | fstream::trunc);
                in_file << text; 
            
                in_file.close();
                return 0;
        }

        in_file.open(path_val, fstream::in | fstream::out | fstream::app);
	
        in_file << key << " : " << value << endl;
        in_file.close();
        return 0;
    }
    //perror("No file available ...");
    return 255;
}