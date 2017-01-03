function mask = remove_green_new(im)

    [h,~,~]=rgb2hsv(im);
    msk = (h>0.22 & h<0.45);
    msk = ~msk;
    figure;imshow(msk)
    msk_f = msk;
    for i = 1:15
        msk_conv = conv2(double(msk_f),ones(3,3),'same');
        msk_f = msk_conv>2;
        figure;imshow(msk_f);
    end

    mask = msk_f;
return