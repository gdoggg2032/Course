path = 'test123.jpg';
psize = [52,64];
Q = enlarge(psize,path);
figure,imshow(Q);
path = '00002.jpg';
psize = [90,68];
P = enlarge(psize,path);
figure,imshow(P);
path = '00483.jpg';
psize = [105,104];
G = enlarge(psize,path);

figure,imshow(G);