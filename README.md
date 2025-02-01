
You said:
# **Project 4: TLB and Virtual Address Translation**

### **Description**
This project involves automating the process of working with the **Translation Lookaside Buffer (TLB), Virtual Address Page Table, and Physical Memory Direct Memory Cache**. The program reads data structures representing these components and determines the byte value at a given virtual address.

### **Features:**
- Reads and stores information from three data structures:
  - **TLB Contents** (Set index, Tag, Physical Page Number)
  - **Virtual Page Table** (Virtual Page Number, Physical Page Number)
  - **Physical Page Cache** (Cache Index, Tag, Byte Offsets)
- Prompts the user for a virtual address (in hex) and determines the corresponding byte value.
- Outputs the byte in hex or prints **"Cannot be determined"** if the data is unavailable.
- Processes three user inputs sequentially.

### **How to Run**
1. Compile the program:
   
```bash
   gcc -o project4 Project4.c
```
2. Run the program:
   
```bash
   ./project4
```

### **Expected Outcome**
- The program correctly parses input data structures.
- The user is prompted for three virtual addresses.
- The program outputs the byte value in hex or states **"Cannot be determined"** if the mapping cannot be completed.

### **Notes**
- Ensure memory management best practices are followed.
- Code should be well-documented with meaningful comments.
- Follow C programming conventions and formatting guidelines.
- Run the program multiple times to verify correct behavior.

### **Deliverables**
- Submit the implementation in Project4.c.

### **Author**
Isabel Agadagba

---
This project demonstrates memory translation, cache lookup, and structured data parsing in C.
