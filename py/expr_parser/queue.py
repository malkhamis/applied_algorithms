class Queue:
    def __init__(self):
        self.items=[]
        self.first=0

    def __compress(self):
        newlst = []
        for i in range(self.first, len(self.items)):
            newlst.append(self.items[i])
        self.items = newlst
        self.first = 0
        
    def isEmpty(self):
        return self.first == len(self.items)

    def enqueue(self,item):
        self.items.append(item)

    def dequeue(self):
        if self.isEmpty(): raise RuntimeError("queue is empty!!")
        if 2*self.first > len(self.items): self.__compress()
        item = self.items[self.first]
        self.first += 1
        return item

    def peak(self):
        if self.isEmpty(): raise RuntimeError("queue is empty!!")
        return self.items[first]
