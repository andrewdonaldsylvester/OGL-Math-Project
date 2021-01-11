#include "matrices.h"
#include <cmath>

mat4 translation(float x, float y, float z)
{
    
    /*
     
      [1  0  0  x]   [X]   [X + x]
      [0  1  0  y] * [Y] = [Y + y]
      [0  0  1  z]   [Z]   [Z + z]
      [0  0  0  1]   [1]   [  1  ]
     
     */
    
    mat4 m_translate = mat4(1.0f);
    
    m_translate[3][0] = x;
    m_translate[3][1] = y;
    m_translate[3][2] = z;
    
    return m_translate;
}

mat4 rotation(float alpha, float beta, float gamma)
{
    
    /*

      Rotation 0 (theta) around the origin:
     
      R(0) = [ cos(0) -sin(0) ] * [X] = [ X cos(a) - Y sin(a) ]
             [ sin(0)  cos(0) ]   [Y]   [ X sin(a) + Y cos(a) ]
     
      Expand this into a 4x4 matrix that fits with homogenous coordinates:
     
             [ cos(0) -sin(0)   0       0  ]
      R(0) = [ sin(0)  cos(0)   0       0  ]
             [   0       0      1       0  ]
             [   0       0      0       1  ]
     
     Moving the position of the sines and cosines we can get rotation around the x, y, and z axis.
     
     R(alpha) * R(beta) * R(gamma) = R
     
     */
    
    mat4 m_alpha = mat4(0.0f);                  mat4 m_beta = mat4(0.0f);                   mat4 m_gamma = mat4(0.0f);
    
    m_alpha[1][1] =  cos( radians(alpha) );     m_beta[0][0] =  cos( radians(beta) );       m_gamma[0][0] =  cos( radians(gamma) );
    m_alpha[2][1] = -sin( radians(alpha) );     m_beta[2][0] = -sin( radians(beta) );       m_gamma[1][0] = -sin( radians(gamma) );
    m_alpha[1][2] =  sin( radians(alpha) );     m_beta[0][2] =  sin( radians(beta) );       m_gamma[0][1] =  sin( radians(gamma) );
    m_alpha[2][2] =  cos( radians(alpha) );     m_beta[2][2] =  cos( radians(beta) );       m_gamma[1][1] =  cos( radians(gamma) );
    
    m_alpha[0][0] = 1;                          m_beta[1][1] = 1;                           m_gamma[2][2] = 1;
    m_alpha[3][3] = 1;                          m_beta[3][3] = 1;                           m_gamma[3][3] = 1;
    
    mat4 m_rotate = m_alpha * m_beta * m_gamma;
    
    return m_rotate;
    
}

mat4 projection(float aspect_ratio, float FOV, float near, float far)
{
/*
 
  FOV is horizontal
  
  aspect ratio is width / height
 
  z just needs to be mapped from (near, far) to (0, 1)
 
    -X ----------------------------------------- +X
  +Z        -1 ______ 0 ______ 1        }
   |          \       |       /          }
   |          -1      0      1            }
   |            \     |     /              }
   |            -1 __ 0 __ 1  }             } far
   |                           }            }
   |               \     /      } near     }
   |                 FOV        }         }
   |                 \ /       }         }
  -Z                 CAM      }         }
 
 
 */
    
    mat4 m_proj = mat4(0.0f);
    
    m_proj[0][0] = 1 / tan( radians( FOV / 2 ) );
    
    m_proj[1][1] = aspect_ratio / tan( radians( FOV / 2 ) );
    
    m_proj[2][2] = 1 / ( far - near );
    m_proj[3][2] = - near / ( far - near );
    
    m_proj[2][3] = 1;
    
    return m_proj;
}

