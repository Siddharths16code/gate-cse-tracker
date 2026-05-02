#pragma once
#include <string>
#include <vector>

struct Topic {
    std::string name;
};

struct Subject {
    std::string name;
    std::string code;
    std::vector<Topic> topics;
};

inline std::vector<Subject> getGateSyllabus() {
    return {
        {
            "Engineering Mathematics", "MATH",
            {
                {"Propositional and First Order Logic"},
                {"Sets, Relations and Functions"},
                {"Partial Orders and Lattices"},
                {"Monoids and Groups"},
                {"Graph Connectivity"},
                {"Graph Matching"},
                {"Graph Colouring"},
                {"Counting and Combinatorics"},
                {"Recurrence Relations"},
                {"Generating Functions"},
                {"Matrices and Determinants"},
                {"System of Linear Equations"},
                {"Eigenvalues and Eigenvectors"},
                {"LU Decomposition"},
                {"Limits, Continuity and Differentiability"},
                {"Maxima and Minima"},
                {"Mean Value Theorem"},
                {"Integration"},
                {"Random Variables"},
                {"Uniform, Normal, Exponential Distributions"},
                {"Poisson and Binomial Distributions"},
                {"Mean, Median, Mode and Standard Deviation"},
                {"Conditional Probability and Bayes Theorem"}
            }
        },
        {
            "Digital Logic", "DL",
            {
                {"Boolean Algebra"},
                {"Combinational Circuits"},
                {"Sequential Circuits"},
                {"Minimization (K-Map, Quine-McCluskey)"},
                {"Number Representations (Fixed Point)"},
                {"Floating Point Arithmetic"},
                {"Computer Arithmetic"}
            }
        },
        {
            "Computer Organization and Architecture", "COA",
            {
                {"Machine Instructions and Addressing Modes"},
                {"ALU Design"},
                {"Data-path and Control Unit"},
                {"Instruction Pipelining"},
                {"Pipeline Hazards"},
                {"Cache Memory"},
                {"Main Memory"},
                {"Secondary Storage"},
                {"I/O Interface - Interrupt Mode"},
                {"I/O Interface - DMA Mode"}
            }
        },
        {
            "Programming and Data Structures", "PDS",
            {
                {"Programming in C"},
                {"Recursion"},
                {"Arrays"},
                {"Stacks"},
                {"Queues"},
                {"Linked Lists"},
                {"Trees"},
                {"Binary Search Trees"},
                {"Binary Heaps"},
                {"Graphs (Representation and Traversal)"}
            }
        },
        {
            "Algorithms", "ALGO",
            {
                {"Searching Algorithms"},
                {"Sorting Algorithms"},
                {"Hashing"},
                {"Asymptotic Worst Case Time Complexity"},
                {"Space Complexity Analysis"},
                {"Greedy Algorithm Design"},
                {"Dynamic Programming"},
                {"Divide and Conquer"},
                {"Graph Traversals (BFS, DFS)"},
                {"Minimum Spanning Trees (Kruskal, Prim)"},
                {"Shortest Paths (Dijkstra, Bellman-Ford, Floyd-Warshall)"}
            }
        },
        {
            "Theory of Computation", "TOC",
            {
                {"Regular Expressions"},
                {"Finite Automata (DFA and NFA)"},
                {"Context-Free Grammars"},
                {"Push-Down Automata"},
                {"Regular Languages"},
                {"Context-Free Languages"},
                {"Pumping Lemma"},
                {"Turing Machines"},
                {"Undecidability"}
            }
        },
        {
            "Compiler Design", "CD",
            {
                {"Lexical Analysis"},
                {"Parsing (Top-down and Bottom-up)"},
                {"Syntax-Directed Translation"},
                {"Runtime Environments"},
                {"Intermediate Code Generation"},
                {"Local Optimization"},
                {"Constant Propagation"},
                {"Liveness Analysis"},
                {"Common Subexpression Elimination"}
            }
        },
        {
            "Operating System", "OS",
            {
                {"System Calls"},
                {"Processes and Threads"},
                {"Inter-Process Communication"},
                {"Concurrency and Synchronization"},
                {"Deadlock (Detection, Prevention, Avoidance)"},
                {"CPU Scheduling"},
                {"I/O Scheduling"},
                {"Memory Management"},
                {"Virtual Memory"},
                {"File Systems"}
            }
        },
        {
            "Databases", "DB",
            {
                {"ER Model"},
                {"Relational Model"},
                {"Relational Algebra"},
                {"Tuple Calculus"},
                {"SQL"},
                {"Integrity Constraints"},
                {"Normal Forms (1NF, 2NF, 3NF, BCNF)"},
                {"File Organization"},
                {"Indexing (B-Trees and B+ Trees)"},
                {"Transactions and Concurrency Control"}
            }
        },
        {
            "Computer Networks", "CN",
            {
                {"OSI and TCP/IP Protocol Stacks"},
                {"Packet, Circuit and Virtual Circuit Switching"},
                {"Data Link Layer - Framing and Error Detection"},
                {"Medium Access Control"},
                {"Ethernet Bridging"},
                {"Routing Protocols - Shortest Path and Flooding"},
                {"Distance Vector and Link State Routing"},
                {"Fragmentation and IP Addressing"},
                {"IPv4 and CIDR Notation"},
                {"ARP, DHCP, ICMP Protocols"},
                {"Network Address Translation (NAT)"},
                {"Transport Layer - Flow and Congestion Control"},
                {"UDP and TCP"},
                {"Sockets"},
                {"Application Layer - DNS, SMTP, HTTP, FTP, Email"}
            }
        }
    };
}
