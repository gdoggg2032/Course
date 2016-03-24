I = imread('test123.jpg');
Size(1) = 52;
Size(2) = 64;

%figure,imshow(I);
rate = 128 / max(Size);
bx(1) = (size(I,1)-Size(1))/2;
bx(2) = bx(1) + Size(1);
by(1) =  (size(I,2)-Size(2))/2;
by(2) = by(1) + Size(2);
II = I(bx(1):bx(2), by(1):by(2));
%figure,imshow(II);
SH = imresize(II,rate,'bilinear');
%figure,imshow(SH);

if size(SH,1) > size(SH,2)
    ra = size(SH,2);
    flag = 1;  
else
    ra = size(SH,1);
    flag = 0;
end;

len = (128 - ra)/2;
if flag == 0
   B = padarray(SH, [len,0], 0);
else
   B = padarray(SH, [0,len], 0);
end
figure,imshow(B);
imwrtie(B,'output.jpg');