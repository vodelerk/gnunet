/*
     This file is part of GNUnet.
     (C) 2009 Christian Grothoff (and other contributing authors)

     GNUnet is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published
     by the Free Software Foundation; either version 2, or (at your
     option) any later version.

     GNUnet is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with GNUnet; see the file COPYING.  If not, write to the
     Free Software Foundation, Inc., 59 Temple Place - Suite 330,
     Boston, MA 02111-1307, USA.
*/

/**
 * @file util/test_container_slist.c
 * @brief Testcases for singly linked lists
 * @author Nils Durner
 */

#include "platform.h"
#include "gnunet_common.h"
#include "gnunet_container_lib.h"

#define ABORT() { fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); return 1; }
#define CHECK(c) { if (! (c)) ABORT(); }

int
main (int argc, char *argv[])
{
  struct GNUNET_CONTAINER_SList *l;
  struct GNUNET_CONTAINER_SList_Iterator *it;
  unsigned int i, j, s;
  const void *p;    

  GNUNET_log_setup ("test-container-slist", "WARNING", NULL);

  l = GNUNET_CONTAINER_slist_create ();
  CHECK (l != NULL);
  CHECK (GNUNET_CONTAINER_slist_count (l) == 0);

  for (i = 0; i < 100; i++)
    GNUNET_CONTAINER_slist_add (l, GNUNET_CONTAINER_SLIST_DISPOSITION_TRANSIENT, &i, sizeof (i));
  CHECK (GNUNET_CONTAINER_slist_count (l) == 100);

  for (it = GNUNET_CONTAINER_slist_begin (l), i = 99;
       GNUNET_CONTAINER_slist_end (it) != GNUNET_YES;
       GNUNET_CONTAINER_slist_next (it), i--)
    {
      p = GNUNET_CONTAINER_slist_get (it, &s);
      CHECK (p != NULL);
      j = *(int *) p;
      CHECK (i == j);
      CHECK (s == sizeof (i));

      j *= 2;
      GNUNET_CONTAINER_slist_insert (it, GNUNET_CONTAINER_SLIST_DISPOSITION_TRANSIENT, &j,
                                     sizeof (j));
    }
  GNUNET_free (it);
  CHECK (GNUNET_CONTAINER_slist_count (l) == 200);
  i = 198;
  CHECK (GNUNET_CONTAINER_slist_contains (l, &i, sizeof (i)));

  for (it = GNUNET_CONTAINER_slist_begin (l);
       GNUNET_CONTAINER_slist_end (it) != GNUNET_YES;)
    {
      p = GNUNET_CONTAINER_slist_get (it, &s);
      CHECK (p != NULL);
      CHECK (s == sizeof (i));
      i = *(int *) p;

      CHECK (GNUNET_CONTAINER_slist_next (it) == GNUNET_YES);
      CHECK (GNUNET_CONTAINER_slist_end (it) != GNUNET_YES);

      p = GNUNET_CONTAINER_slist_get (it, &s);
      CHECK (p != NULL);
      CHECK (s == sizeof (j));
      j = *(int *) p;

      CHECK (j * 2 == i);

      GNUNET_CONTAINER_slist_erase (it);
    }
  GNUNET_free (it);
  CHECK (GNUNET_CONTAINER_slist_count (l) == 100);
  i = 99;
  CHECK (GNUNET_CONTAINER_slist_contains (l, &i, sizeof (i)) == GNUNET_NO);
  i = 198;
  CHECK (GNUNET_CONTAINER_slist_contains (l, &i, sizeof (i)));

  GNUNET_CONTAINER_slist_clear (l);
  CHECK (GNUNET_CONTAINER_slist_count (l) == 0);

  for (i = 0; i < 100; i++)
    GNUNET_CONTAINER_slist_add (l, GNUNET_CONTAINER_SLIST_DISPOSITION_TRANSIENT, &i, sizeof (i));

  GNUNET_CONTAINER_slist_destroy (l);

  return 0;
}
