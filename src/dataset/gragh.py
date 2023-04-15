import networkx as nx
import random

# Define a function to generate a graph with n nodes and an average out-degree of 1.5
def generate_graph(n):
    # Create an empty directed graph
    G = nx.DiGraph()
    # Add n nodes to the graph
    for i in range(n):
        G.add_node(i)
    # Add edges to the graph to achieve an average out-degree of 1.5
    for node in G.nodes():
        target = random.choice(list(G.nodes()))
        G.add_edge(node, target)
    for node in G.nodes():
        while G.out_degree(node) < 1.5:
            target = random.choice(list(G.nodes()))
            if target != node and not G.has_edge(node, target):
                G.add_edge(node, target)
                if random.random() > 1/3:
                    break
    # Return the generated graph
    return G

# Generate graphs with 100, 200, 400, 1000, 2000, 4000 nodes
for n in [50, 150]:
    # Generate a graph with n nodes and an average out-degree of 1.5
    G = generate_graph(n)
    # Save the graph to a file
    filename = "graph_{}_nodes_1.5_out_degree.txt".format(n)
    with open(filename, "w") as f:
        for edge in G.edges():
            f.write(str(edge[0]) + "," + str(edge[1]) + "\n")
