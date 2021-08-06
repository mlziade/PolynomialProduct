import matplotlib.pyplot as plt
  
# Brute force - x and y values
bfx = [2,4,1]
bfy = [1, 2, 3]

# Divide and conquer 4 - x and y values
cd4x = [2,4,1]
cd4y = [2, 3, 4]

# Divide and conquer 3 - x and y values
cd3x = [2,4,1]
cd3y = [3, 4, 5]

# plotting the points 
plt.plot(bfx, bfy, label = "Brute Force")
plt.plot(cd4x, cd4y, label = "Divide and Conquer 4")
plt.plot(cd3x, cd3y, label = "Divide and Conquer 3")
  
# label naming
plt.xlabel('Grau - int')
plt.ylabel('Tempo de execucao - float')
  
# graph title
plt.title('Grafico performance')

# show a legend
plt.legend()
  
# show graph
plt.show()