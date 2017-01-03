function [img_ng, mask, img_g] = remove_green(img,para)

    [m,n,ch] = size(img);

    img_grey = rgb2gray(img);

    nb_reg = img_grey > (3/255);

%     g_reg_1 = (img(:,:,2) - (img(:,:,1) + img(:,:,3))) > 0;
    
    g_reg_2 = (img(:,:,2) - img(:,:,1))>0;
    g_reg_3 = (img(:,:,2) - img(:,:,3))>0;
    g_reg_4 = (img(:,:,2) - para*(img(:,:,1) + img(:,:,3))) > 0;
    
    g_reg = (uint8(g_reg_4) + uint8(g_reg_2) + uint8(g_reg_3)) == 3;
    ng_reg = (~nb_reg) + (~g_reg);
    mask = ng_reg;
    
    img_ng = zeros(m,n,ch);
    
    img_ng(:,:,1) = img(:,:,1) .* mask;
    img_ng(:,:,2) = img(:,:,2) .* mask;
    img_ng(:,:,3) = img(:,:,3) .* mask;
    
    img_g = img - img_ng;
    
return

